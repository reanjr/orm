<?php
/**
 * Copyright Skinit, Inc.
 */

class Condition
{
    const OP_EQUAL = "=";
    const OP_NOT_EQUAL = "!=";
    
    protected $name = null;
    protected $value = null;
    protected $op = null;
    
    /**
     * Optionally set the name, value, and operator for this condition.  If name is not provided, the condition may
     * receive one when added to a filter with a name.  If the value is null, the condition will apply to the filter a
     * check for null values.  If you no longer wish to use a condition, remove it rather than set it to null.  The
     * default operator is an equality test.
     * 
     * @param   string  $name
     * @param   mixed   $value
     * @param   string  $op
     */
    public function __construct( $name = null, $value = null, $op = Condition::OP_EQUAL )
    {
        $this->setName( $name );
        $this->setValue( $value );
        $this->setOp( $op );
    }
    
    /**
     * Set the name for the condition.  This is the tuple value to be filtered by the condition.  The name may be set
     * to null as long it is used in a filter as a named condition.  Return this instance for chaining.
     * 
     * @param   string      $name
     * @return  Condition
     */
    public function setName( $name = null )
    {
        $this->name = is_null($name) ? null : (string)$name;
        return $this;
    }
    
    /**
     * Set the value for the condition.  This is the value against which the tuple value is compared.  Setting the
     * value to null will test specifically for null values.  Return this instance for chaining.
     * 
     * @param   mixed       $value
     * @return  Condition
     */
    public function setValue( $value = null )
    {
        $this->value = is_object($value) ? (string)$value : $value;
        return $this;
    }
    
    /**
     * Set the operator used to compare the tuple value with the condition value.  This should be one of the
     * Condition::OP_* constants.  Return this instance for chaining.
     * 
     * @param   string      $op
     * @return  Condition
     */
    public function setOp( $op )
    {
        $this->op = $op;
        return $this;
    }
    
}