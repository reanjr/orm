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
     *
     * @param   ISchematic  $schema
     * @param   string      $name
     */
    public function __construct( ISchematic $schema, $name )
    {
        $this->schema = $schema;
        $this->name = (string)$name;
    }

    /**
     * Return the schema in which this relation is found.
     *
     * @return  ISchematic
     */
    public function getSchema()
    {
        return $this->schema;
    }

    /**
     * Return the name of this relation.
     *
     * @return  string
     */
    public function getName()
    {
        return $this->name;
    }
    
    /**
     * Return the attributes defined on this relation.
     * 
     * @return  array 
     */
    public function getAttributes()
    {
        return $this->getSchema()->getRelationAttributes( $this->getName() );
    }
    
    /**
     * Return the attributes defined on this relation that are part of the key.
     * 
     * @return  array
     */
    public function getKeyAttributes()
    {
        return $this->getSchema()->getRelationKeyAttributes( $this->getName() );
    }

    /**
     * Return the relationships defined on this relation.
     *
     * @return  array
     */
    public function getRelationships()
    {
        return $this->getSchema()->getRelationships( $this->getName() );
    }

    /**
     * Format this relation as a string.
     *
     * @return  string
     */
    public function __toString()
    {
        return $this->getName();
    }
    
}