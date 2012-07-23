<?php
/**
 * Copyright Skinit, Inc.
 */

class OrmSchemaException extends OrmException
{
    
    public function __construct( $message )
    {
        parent::__construct( "Schema Error: $message" );
    }
    
}