<?php
/**
 * Copyright Skinit, Inc.
 */

class OrmModelBuilder
{
    protected $properties = array();

    /**
     * Create a new instance and return it for chaining.
     *
     * @return  OrmModelBuilder
     */
    public static function create()
    {
        return new OrmModelBuilder();
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