<?php
/**
 * Copyright Skinit, Inc.
 */

class SchemaRelation
{
    protected $schema = null;
    protected $name = null;

    /**
     * Set the schema and name for this relation.
     */
    public function __construct( ISchematic $schema, $name )
    {
        $this->schema = $schema;
        $this->name = (string)$name;
    }
    
}