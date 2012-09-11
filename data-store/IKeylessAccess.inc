<?php
/**
 * Copyright Skinit, Inc.
 */

/**
 * Interface for a data store supporting keyless access to the all contained data.
 */
interface IKeylessAccess
{
    
    /**
     * Return an iterator that can provide access to all records for a relation.
     * 
     * @param   string      $relation
     * @return  Iterator
     */
    function findAll( $relation );
    
}