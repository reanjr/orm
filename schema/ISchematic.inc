<?php
/**
 * Copyright Skinit, Inc.
 */

interface ISchematic
{
    const TYPE_TEXT = 'text';
    const TYPE_INTEGER = 'integer';
    const TYPE_NUMBER = 'number';
    const TYPE_FLAG = 'flag';
    const TYPE_DATE_TIME = 'datetime';

    /**
     * Return the name of the schema.
     *
     * @return  string
     */
    function getName();

    /**
     * Return the list of relations (tables in SQL) in this data store as an array of SchemaRelation instances.
     *
     * @return  array
     */
    function getRelations();

    /**
     * Return a list of attributes (columns in SQL) for the provided relat(ion as an array of SchemaAttribute
     * instances.
     *
     * @param   string  $relationName
     * @return  array
     */
    function getRelationAttributes( $relationName );

    /**
     * Return an array of relationships (foreign key constraints in SQL) for the provided relation.  Each relationship
     * is
     *
     * @param   string  $relation
     * @return  
     */
    
}
