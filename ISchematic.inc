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
     * Return an array of the names of the relations (tables in SQL) in this data store.
     *
     * @return  array
     */
    function getRelations();

    /**
     * Return an array of attribute names (column names in SQL) for the provided relat(ion.
     *
     * @param   string  $relation
     * @return  array
     */
    function getRelationAttributes( $relation );

    /**
     * Return the data type for the provided relation attribute.  This will be one of the ISchematic::TYPE_* constants.
     *
     * @param   string  $relation
     * @param   string  $attribute
     * @return  string
     */
    function getRelationAttributeType( $relation, $attribute );
    
}