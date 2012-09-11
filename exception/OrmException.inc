<?php
/**
 * Copyright Skinit, Inc.
 */

class OrmException extends Exception
{
    
    public function __construct( $message )
    {
        parent::__construct( "[ORM] $message", 0, null );
    }
    
}