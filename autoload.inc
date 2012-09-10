<?php
/**
 * Copyright Skinit, Inc.
 */

function __autoload( $className )
{
    $directory = new RecursiveDirectoryIterator( "." );
    $flattened = new RecursiveIteratorIterator( $directory );
    $filtered = new RegexIterator( $flattened, '@^.*\.inc$@', RecursiveRegexIterator::GET_MATCH );
    
    $classFile = "$className.inc";
    foreach( $filtered as $results )
    {
        $file = array_shift( $results );
        if( basename( $file ) == $classFile )
        {
            require_once( $file );
            return;
        }
    }
}