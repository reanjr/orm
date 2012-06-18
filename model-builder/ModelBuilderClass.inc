<?php
/**
 * Copyright Skinit, Inc.
 */

class ModelBuilderClass
{
    protected $name = null;
    protected $properties = array();
    
    /**
     * Set the name of the class.
     * 
     * @param   string  $name 
     */
    public function __construct( $name )
    {
        $this->name = (string)$name;
    }
    
    /**
     * Return the name of this class.
     * 
     * @return  string
     */
    public function getName()
    {
        return $this->name;
    }
    
    /**
     * Add the provided property to this instance.
     * 
     * @param   ModelBuilderProperty    $property 
     */
    public function addProperty( ModelBuilderProperty $property )
    {
        if( array_key_exists( (string)$property, $this->properties ) )
        {
            throw new Exception( "duplicate property name '$property'" );
        }
        
        $this->properties[(string)$property] = $property;
    }
    
    /**
     * Create a new property, associate it with this instance, and return it.
     * 
     * @param   string  $propertyName
     * @param   string  $dataType
     */
    public function createProperty( $propertyName, $dataType )
    {
        $property = new ModelBuilderProperty( $propertyName, $dataType );
        $this->addProperty( $property );
        return $property;
    }
    
    /**
     * Render this class as a string.
     * 
     * @return  string
     */
    public function __toString()
    {
        return $this->getName();
    }
    
    /**
     * Generate PHP code for this class and return it.
     * 
     * @return  string 
     */
    public function getCode()
    {
        $name = $this->getName();
        $code = <<<CODE
abstract class $name extends OrmModel
{
}
CODE;
        return $code;
    }
    
}