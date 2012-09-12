<?php
/**
 * Copyright Skinit, Inc.
 */

interface ISearchableDataStore
{

    /**
     * Retrieve a set of tuples (records in SQL) from a relation in the data store and return an iterator for them.  If
     * a filter is provided, include only tuples that match the filter.  The caller may specify any number of filters
     * as additional arguments.  If multiple filters are specified, include tuple that match any of the filters.
     *
     * @param   string  $relationName
     * @param   Filter  $filter_1
     */
    function find( $relationName, Filter $filter_1 = null );

}
