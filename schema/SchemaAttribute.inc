<?php
/**
 * Copyright Skinit, Inc.
 */

class SchemaAttribute
{
    protected $schema = null;
    protected $relationName = null;
    protected $name = null;
    protected $dataType = null;

    /**
     *
     */
    public function __construct( ISchematic $schema, $relationName, $name, $dataType )
    {
        
    }
    
}