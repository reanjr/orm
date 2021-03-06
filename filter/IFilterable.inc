<?php
/**
 * Copyright Skinit, Inc.
 */

interface IFilterable
{
    
    /**
     * Add a filter with one or more conditions and return the filter.  See Filter::create() for more information on
     * the optional ways to pass in conditions.
     * 
     * Classes wishing to implement the IFilterable interface may use the following code to create a basic filter based
     * on the provided arguments:
     * 
     *   Filter::createProxy( $this )->addTests( func_get_args() );
     * 
     * @param   mixed   $condition_1
     * @return  Filter
     */
    function filter( $condition_1 );
    
    /**
     * Add an inverted filter with one or more conditions and return the filter.  See the Filter constructor for more
     * information on the optional ways to pass in conditions.
     * 
     * Classes wishing to implement the IFilterable interface may use the following code to create a basic inverted
     * filter based on the provided arguments:
     * 
     *   Filter::createProxy( $this )->addTests( func_get_args() )->invert();
     * 
     * @param   mixed   $condition_1
     * @return  Filter
     */
    function filterOut( $condition_1 );

    /**
     * Return the filters added to this instance.
     * 
     * @return  array
     */
    function getFilters();
    
}