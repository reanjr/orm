<?php
/**
 * Copyright Skinit, Inc.
 */

abstract class OrmDataStore
{
    
    /**
     * Connect to the data store.  Throw an exception if no connection can be established.
     */
    abstract public function connect();

    /**
     * 
     */

}