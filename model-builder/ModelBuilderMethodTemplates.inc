<?php
/**
 * Copyright Skinit, Inc.
 */

class ModelBuilderMethodTemplates
{
    
    /**
     * Return the friendly name for this instance.
     * 
     * @return object
     */
    public function readValue_getLogicalName()
    {
        return LITERAL;
    }
    
    /**
     * Return friendly name property.
     * 
     * @return object
     */
    public function simpleGetter_getLogicalName()
    {
        // return value
        return $this->logicalName;
    }
    
    /**
     * Set the friendly name property.
     * 
     * @param object $logicalName 
     */
    public function simpleSetter_setLogicalName( $logicalName )
    {
        // ensure logicalName is of the correct type
        $this->logicalName = (object)$logicalName;
    }
    
    /**
     * Get the associated friendly name.
     * 
     * @return object
     */
    public function relatedGetter_getLogicalName()
    {
        // if we don't have an instance, try to load one
        if( is_null($this->$logicalName) )
        {
            $id = $this->getFromKey();
            $this->logicalName = new object( $id );
        }
        
        // return loaded instance
        return $this->logicalName;
    }
    
    /**
     * Set the associated friendly name.
     * 
     * @param object $logicalName
     */
    public function relatedSetter_setLogicalName( object $logicalName = null )
    {
        // handle associated from key value
        $this->fromKey = ( is_null($logicalName) )
            ? null
            : $logicalName->getToKey();
        
        // set value
        $this->logicalName = $logicalName;
    }
    
}