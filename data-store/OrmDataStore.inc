<?php
/**
 * Copyright Skinit, Inc.
 */

abstract class OrmDataStore
{
    
    /**
     * Connect to the data store.  Throw an exception if no connection can be established.
     */
    public function connect();

    /**
     * Fetch a record from the data store and return it as a key-value array.
     * 
     * @param   string  $relationName
     * @param   mixed   $key
     */
    abstract public function fetch( $relationName, $key );
    
}