<?php
/**
 * Copyright Skinit, Inc.
 */

class OrmUnknownDataStoreException extends OrmException
{
    
    public function __construct( $name )
    {
        parent::__construct( "Unknown Data Store: $name" );
    }
    
}