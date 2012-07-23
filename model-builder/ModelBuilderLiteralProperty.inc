<?php
/**
 * Copyright Skinit, Inc.
 */

class ModelBuilderLiteralProperty extends ModelBuilderProperty
{
    protected $value = null;
    
    /**
     * Set the name and value of the property.
     * 
     * @param   string  $name
     * @param   mixed   $value
     */
    public function __construct( $name, $value )
    {
        $this->name = (string)$name;
        
        switch( gettype($value) )
        {
            case "boolean":
                $this->value = $value ? "true" : "false";
                $this->dataType = "bool";
                break;
            case "string":
                $this->value = '"' . str_replace( '"', '\"', $value ) . '"';
                $this->dataType = "bool";
                break;
            case "integer":
                $this->value = (string)$value;
                $this->dataType = "int";
                break;
            case "double":
                $this->value = (string)$value;
                $this->dataType = "float";
            default:
                $this->value = "null";
                $this->dataType = null;
        }
    }
    
    /**
     * Return the value of this property.
     * 
     * @return  mixed 
     */
    public function getValue()
    {
        return $this->value;
    }
    
}