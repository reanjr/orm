<?php
/**
 * Copyright Skinit, Inc.
 */

abstract class OrmDataStore
{
    protected $name = null;
    
    /**
     * Connect to the data store.  Throw an exception if no connection can be established.
     */
    public function connect()
    {
    }
    
    /**
     * Fetch a record from the data store and return it as a key-value array.
     * 
     * @param   string  $relation
     * @param   mixed   $key
     */
    abstract public function fetch( $relation, $key );
    
    /**
     * Return the name of the data store.
     * 
     * @return  string
     */
    public function getName()
    {
        return $this->name;
    }
    
    /**
     * Set the name of the data store.
     * 
     * @param   string  $name
     */
    public function setName( $name )
    {
        $this->name = (string)$name;
    }
    
}