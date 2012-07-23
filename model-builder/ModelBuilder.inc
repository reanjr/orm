<?php
/**
 * Copyright Skinit, Inc.
 */

class ModelBuilder
{
    protected $classes = array();

    /**
     * Create a new instance and return it for chaining.
     *
     * @return  SchemaModelBuilder
     */
    public static function create()
    {
        return new ModelBuilder();
    }

    /**
     * Add models for the provided schema.
     * 
     * @param   ISchematic  $schema 
     */
    public function addSchema( ISchematic $schema )
    {
        $function = array( $this, 'addRelation' );
        array_walk( $schema->getRelations(), $function );
    }
    
    /**
     * Add a model for the provided relation.
     * 
     * @param   SchemaRelation  $relation 
     */
    public function addRelation( SchemaRelation $relation )
    {
        $className = ucfirst( Orm::formatLogicalName( $relation ) ) . "Model";
        $baseClassName = $className . "Base";
        
        $baseClass = $this->createClass( $baseClassName )->extend( "OrmModel" )->setAbstract( true );
        foreach( $relation->getAttributes() as $attribute )
        {
            $this->addAttribute( $attribute, $baseClassName );
        }
        
        $class = $this->createClass( $className )->extend( $baseClassName );
    }
    
    /**
     * Add a property for the provided attribute to the class specified.
     * 
     * @param   SchemaAttribute     $attribute
     * @param   string              $className
     */
    public function addAttribute( SchemaAttribute $attribute, $className )
    {
        if( !isset($this->classes[(string)$className]) )
        {
            throw new Exception( "unknown class '$className'" );
        }
        $class = $this->classes[(string)$className];
        
        // map schematic types to PHP types
        switch( $attribute->getDataType() )
        {
            case ISchematic::TYPE_DATE_TIME:
            case ISchematic::TYPE_INTEGER:
                $dataType = 'int';
                break;
            case ISchematic::TYPE_FLAG:
                $dataType = 'bool';
                break;
            case ISchematic::TYPE_NUMBER:
                $dataType = 'float';
                break;
            case ISchematic::TYPE_TEXT:
                $dataType = 'string';
                break;
            default:
                throw new Exception( "unknown data type '{$attribute->getDataType()}'" );
        }
        $class->createProperty( Orm::formatLogicalName( $attribute->getName() ), $dataType );
        $class->createMethod( 'simpleGetter', $attribute );
        $class->createMethod( 'simpleSetter', $attribute );
    }
    
    /**
     * Add the provided model class to this instance.
     * 
     * @param   ModelBuilderClass   $class 
     */
    public function addClass( ModelBuilderClass $class )
    {
        if( array_key_exists( (string)$class, $this->classes ) )
        {
            throw new Exception( "duplicate class name '$class'" );
        }
        
        $this->classes[(string)$class] = $class;
    }
    
    /**
     * Create a new model class, associate it with this instance, and return it.
     * 
     * @param   string              $className
     * @return  ModelBuilderClass
     */
    public function createClass( $className )
    {
        $class = new ModelBuilderClass( $className );
        $this->addClass( $class );
        return $class;
    }
    
    /**
     * Return the classes associated with this instance.
     * 
     * @return  array 
     */
    public function getClasses()
    {
        return array_values( $this->classes );
    }
    
    /**
     * Parse the ModelBuilderMethodTemplates class and return the specified method template.
     * 
     * @param   string  $templateName 
     * @return  string
     */
    public static function getMethodTemplate( $templateName )
    {
        // ensure template class is loaded
        $m = new ModelBuilderMethodTemplates();
        
        // find template class file and parse its contents
        $reflector = new ReflectionClass( $m );
        $filePath = $reflector->getFileName();
        $file = file_get_contents( $filePath );
        
        // create extraction pattern
        $pattern = "(?P<comment>\s*/\*\*([^*]|\*(?!/))*?\n\s*\*/\s*\n)?";
        $pattern .= "(?P<lead>\s*)(?P<descriptor>(public|private|protected|abstract|final|static) )*";
        $pattern .= "function (?P<template>[^_]+)_(?<method>[^_]+)\s*\(.*?\n(?P=lead)}";
        $pattern = "@$pattern@s";
        
        // find template
        if( preg_match_all( $pattern, $file, $matches, PREG_PATTERN_ORDER ) )
        {
            if( false === ( $index = array_search( $templateName, $matches['template'] ) ) )
            {
                throw new Exception( "could not find '$templateName' template" );
            }
            
            $template = $matches[0][$index];
            $template = str_replace( "function $templateName" . "_", "function ", $template );
            return $template;
        }
        throw new Exception( "could not find '$templateName' template" );
    }
    
    /**
     * Return the values that will be replaced in a method template.
     * 
     * @return  array
     */
    public static function getTemplateValues( $name, $type )
    {
        return array(
            "friendly name" => strtolower( Orm::formatFriendlyName( $name ) ),
            "Friendly Name" => Orm::formatFriendlyName( $name ),
            "logicalName" => Orm::formatLogicalName( $name ),
            "LogicalName" => ucfirst( Orm::formatLogicalName( $name ) ),
            "physical_name" => Orm::formatPhysicalName( $name ),
            "(object)" => in_array( $type, array("int", "string", "bool", "array") ) ? "($type)" : "",
            "object" => $type,
        );
    }
    
}