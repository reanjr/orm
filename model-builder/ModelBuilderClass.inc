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
    protected $properties = array();
    protected $methods = array();
    
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
     * @param   bool    $value
     */
    public function setAbstract( $value )
    {
        $this->isAbstract = (bool)$value;
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
        switch( gettype($literal) )
        {
            case "string":
                $replace["LITERAL"] = '"' . str_replace( '"', '\"', $literal ) . '"';
                $dataType = $dataType ?: "string";
                break;
            case "integer":
                $replace["LITERAL"] = (string)$literal;
                $dataType = $dataType ?: "int";
            case "double":
                $replace["LITERAL"] = (string)$literal;
                $dataType = $dataType ?: "float";
                break;
            case "boolean":
                $replace["LITERAL"] = $literal ? "true" : "false";
                $dataType = $dataType ?: "bool";
                break;
            default:
                $replace["LITERAL"] = "null";
        }
        
        // if the object has a data type, set up the replacement
        if( $dataType )
        {
            $replace['object'] = $dataType;
            $replace["(object)"] = in_array( $dataType, array("int", "string", "bool", "array") ) ? "($dataType)" : "";
            $replace["object"] = $dataType;
        }
        
        // get template and make replacements
        $template = ModelBuilder::getMethodTemplate( $templateName );
        $template = str_replace( array_keys( $replace ), array_values( $replace ), $template );
        
        // add final method to this instance
        $this->methods[] = $template;
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
        $propertiesString = $this->getPropertiesCode();
        $methodsString = $this->getMethodsCode();
        
        $code = <<<CODE
<?php
{$descriptorString}class $name $extendsString$implementsString
{
$propertiesString
$methodsString

}
CODE;
        return $code;
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
