<?php
/**
 * Copyright Skinit, Inc.
 */

class OrmMissingDriverException extends OrmException
{
    
    public function __construct( $name )
    {
        parent::__construct( "Missing Driver: $name" );
    }
    
}