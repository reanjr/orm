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
        $class->createProperty( $attribute->getName(), $dataType );
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
     * Add a new property.  The data type should be on of the ISchematic::TYPE_* constants or a model class name.  If a
     * property with the specified name already exists, do nothing.
     * 
     * @param   string  $name
     * @param   string  $dataType
     */
    public function addProperty( $name, $dataType )
    {
        $this->properties[(string)$name] = (string)$dataType;
    }
    
}