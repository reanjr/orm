<?php
/**
 * Copyright Skinit, Inc.
 */

class ModelBuilderClass
{
    protected $name = null;
    protected $isAbstract = false;
    protected $extend = null;
    protected $implement = array();
    
    protected $constants = array();
    protected $properties = array();
    protected $methods = array();
    
    protected $metadata = array();
    
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
     * Set whether this class is abstract and return this instance for chaining.
     * 
     * @param   bool                $value
     * @return  ModelBuilderClass
     */
    public function setAbstract( $value )
    {
        $this->isAbstract = (bool)$value;
        return $this;
    }
    
    /**
     * Set the class from which this class inherits and return this instance for chaining.
     * 
     * @param   string              $className
     * @return  ModelBuilderClass 
     */
    public function extend( $className )
    {
        $this->extend = (string)$className;
        return $this;
    }
    
    /**
     * Add an interface which is implemented by this class and return this instance for chaining.
     * 
     * @param   string              $interfaceName
     * @return  ModelBuilderClass 
     */
    public function implement( $interfaceName )
    {
        $this->implement[(string)$interfaceName] = (string)$interfaceName;
    }
    
    /**
     * Add the provided constant to the class.
     * 
     * @param   ModelBuilderConstant    $constant
     */
    public function addConstant( $constant )
    {
        if( array_key_exists( (string)$constant, $this->constants ) )
        {
            throw new Exception( "duplicate constant name '$constant'" );
        }
        
        $this->constants[(string)$constant] = $constant;
    }
    
    /**
     * Create a new constant, associate it with this instance, and return it.
     * 
     * @param   string                  $constantName
     * @param   mixed                   $value
     * @return  ModelBuilderConstant
     */
    public function createConstant( $constantName, $value )
    {
        $constant = new ModelBuilderConstant( $constantName, $value );
        $this->addConstant( $constant );
        return $constant;
    }
    
    /**
     * Return a list of constants associated with this instance.
     * 
     * @return  array
     */
    public function getConstants()
    {
        return $this->constants;
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
     * @param   string                  $propertyName
     * @param   string                  $dataType
     * @return  ModelBuilderProperty
     */
    public function createProperty( $propertyName, $dataType )
    {
        $property = new ModelBuilderProperty( $propertyName, $dataType );
        $this->addProperty( $property );
        return $property;
    }
    
    /**
     * Return a list of properties associated with this instance.
     * 
     * @return  array 
     */
    public function getProperties()
    {
        return $this->properties;
    }
    
    /**
     * Create a new method based on the provided template and object.
     * 
     * @param   string  $templateName
     * @param   object  $object 
     */
    public function createMethod( $templateName, $name, $dataType = null, $literal = null )
    {
        // create list of replacements
        $replace = array();
        $replace["RAW_NAME"] = $name;
        $replace['friendly name'] = strtolower(Orm::formatFriendlyName( $name ));
        $replace["Friendly Name"] = Orm::formatFriendlyName( $name );
        $replace['logicalName'] = Orm::formatLogicalName( $name );
        $replace['LogicalName'] = ucfirst(Orm::formatLogicalName( $name ));
        $replace['physical_name'] = Orm::formatPhysicalName( $name );
        
        // format the literal value
        $replace["LITERAL"] = ModelBuilder::formatPhpLiteral( $literal, $dataType );
        
        // if the object has a data type, set up the replacement
        if( $dataType )
        {
            $replace['object'] = $dataType;
            $replace["(object)"] = in_array( $dataType, array("int", "string", "bool", "array") ) ? "($dataType)" : "";
            $replace["object"] = $dataType;
        }
        
        // otherwise, make sure we fix any errant casts to null
        else
        {
            $replace["(object)"] = "";
        }
        
        // get template and make replacements
        $template = ModelBuilder::getMethodTemplate( $templateName );
        $template = str_replace( array_keys( $replace ), array_values( $replace ), $template );
        
        // add final method to this instance
        $this->methods[] = $template;
    }
    
    /**
     * Set metadata for the class.  If there is already a value for the specified metadata value, it will be
     * overwritten.  The template methods may use this metadata to control modify their behavior depending on the model
     * structure.
     * 
     * @param   string  $name
     * @param   mixed   $value
     */
    public function setMetadata( $name, $value )
    {
        $this->metadata[(string)$name] = $value;
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
        $descriptorString = $this->isAbstract
          ? "abstract "
          : "";
        $extendsString = $this->extend
          ? "extends " . $this->extend . " "
          : "";
        $implementsString = $this->implement
          ? "implements " . implode( ", ", $this->implement ) . " "
          : "";
        $constantsString = $this->getConstantsCode();
        $propertiesString = $this->getPropertiesCode();
        $methodsString = $this->getMethodsCode();
        
        $code = <<<CODE
<?php
{$descriptorString}class $name $extendsString$implementsString
{
$constantsString

$propertiesString
$methodsString

}
CODE;
        return $code;
    }
    
    /**
     * Generate PHP code detailing this class's constants and return it
     * 
     * @return  string
     */
    protected function getConstantsCode()
    {
        $constantsStrings = array_map( function(ModelBuilderConstant $constant) {
            $value = $constant->getPhpValue();
            $name = $constant->getName();
            $friendlyName = strtolower( Orm::formatFriendlyName( $name ) );
            return "    /** constant for $friendlyName attribute name */\n" .
                   "    const $name = $value;\n";
        }, $this->getConstants() );
        
        return trim(implode( "\n", $constantsStrings ), "\n");
    }
    
    /**
     * Generate PHP code detailing this class's properties and return it
     * 
     * @return  string
     */
    protected function getPropertiesCode()
    {
        $propertiesStrings = array_map( function(ModelBuilderProperty $property) {
            $dataType = $property->getDataType();
            $name = $property->getName();
            return "    /** @var $dataType \$$name */\n" .
                   "    protected \$$name = null;\n";
        }, $this->getProperties() );
        
        return trim(implode( "\n", $propertiesStrings ), "\n");
    }
    
    /**
     * Generate the code for the PHP methods for this class and return it.
     * 
     * @return  string 
     */
    public function getMethodsCode()
    {
        return implode( "", $this->methods );
    }
}
