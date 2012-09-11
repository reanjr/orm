<?php
/**
 * Copyright Skinit, Inc.
 */

class ModelBuilderConstant
{
    protected $name = null;
    protected $dataType = null;
    protected $value = null;
    
    /**
     * Set the name and value of the constant.
     * 
     * @param   string  $name 
     * @param   mixed   $value
     */
    public function __construct( $name, $value )
    {
        $this->name = (string)$name;
        $this->value = $value;
        
        // determine data type and cast value as needed to normalize
        if( is_int($value) )
        {
            $this->dataType = "int";
        }
        elseif( is_double($value) )
        {
            $this->dataType = "float";
        }
        elseif( is_bool($value) )
        {
            $this->dataType = "bool";
        }
        elseif( is_null($value) )
        {
            $this->dataType = "null";
        }
        else
        {
            $this->dataType = "string";
            $this->value = (string)$value;
        }
    }
    
    /**
     * Return the name of this constant.
     * 
     * @return  string 
     */
    public function getName()
    {
        return $this->name;
    }
    
    /**
     * Return the data type of this constant. 
     * 
     * @return  string
     */
    public function getDataType()
    {
        return $this->dataType;
    }
    
    /**
     * Return the value of this constant.
     * 
     * @return  mixed
     */
    public function getValue()
    {
        return $this->value;
    }
    
    /**
     * Return a PHP literal for the constant value.
     * 
     * @return  string
     */
    public function getPhpValue()
    {
        return $this->getDataType() == "string"
          ? '"' . str_replace( '"', '\"', $this->getValue() ) . '"'
          : ($this->getDataType() == "null" ? "null" : (string)$this->getValue());
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