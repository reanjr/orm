<?php
/**
 * Copyright Skinit, Inc.
 */

class OrmFilterParseException extends OrmException
{
    
    public function __construct( $message )
    {
        parent::__construct( "Filter Parse Error: $message" );
    }
        
}