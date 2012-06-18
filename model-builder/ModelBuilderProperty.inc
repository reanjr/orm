<?php
/**
 * Copyright Skinit, Inc.
 */

class ModelBuilderProperty
{
    protected $name = null;
    protected $dataType = null;
    
    /**
     * Set the name and data type of the property.
     * 
     * @param   string  $name 
     * @param   string  $dataType
     */
    public function __construct( $name, $dataType )
    {
        $this->name = (string)$name;
        $this->dataType = (string)$dataType;
    }
    
    /**
     * Return the name of this property.
     * 
     * @return  string 
     */
    public function getName()
    {
        return $this->name;
    }
    
    /**
     * Return the data type of this property. 
     * 
     * @return  string
     */
    public function getDataType()
    {
        return $this->dataType;
    }

    /**
     * Render this property as a string.
     * 
     * @return  string
     */
    public function __toString()
    {
        return $this->getName();
    }
    
}