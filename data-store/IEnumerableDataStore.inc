<?php
/**
 * Copyright Skinit, Inc.
 */

/**
 * Interface for a data store supporting enumeration of all contained data.
 */
interface IEnumerableDataStore
{
    
    /**
     * Return an iterator that can provide access to all records for a relation.
     * 
     * @param   string      $relationName
     * @return  Iterator
     */
    function findAll( $relationName );
    
}