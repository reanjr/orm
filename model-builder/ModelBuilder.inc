<?php
/**
 * Copyright Skinit, Inc.
 */

class ModelBuilder
{
    protected $properties = array();

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
    }
    
    /**
     * Build a model for the provided relation. 
     */
    
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