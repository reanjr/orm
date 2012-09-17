<?php
/**
 * Copyright Skinit, Inc.
 */

class ProxyFilter extends Filter implements IFilterable
{
    protected $proxyTarget = null;
    
    /**
     * Set the proxy target object and any number of filter tests.  The arguments for tests is the same as that for the
     * Filter constructor.  An assumption is made by the ProxyFilter that the target will generate ProxyFilter instances
     * when forwarded requests to create new filters.
     * 
     * @param   IFilterable     $target
     * @param   mixed           $test_1
     */
    public function __construct( IFilterable $target, $test_1 )
    {
        $this->setProxyTarget( $target );
        
        // strip the first argument and pass the rest to addTests
        $args = func_get_args();
        array_shift( $args );
        $this->addTests( $args );
    }
    
    /**
     * Set the filterable object this instance is proxying for.  All IFilterable calls will be forwarded to this object.
     * Return this instance for chaining.
     * 
     * @param   IFilterable     $target
     * @return  ProxyFilter
     */
    public function setProxyTarget( IFilterable $target )
    {
        $this->proxyTarget = $target;
        return $this;
    }
    
    /**
     * Return the filterable object this instance is proxying for.
     * 
     * @return  IFilterable
     */
    public function getProxyTarget()
    {
        return $this->proxyTarget;
    }
    
    
    /**************************************************************************
     * IFilterable implementation
     **************************************************************************/
    
    /**
     * @param   mixed       $condition_1
     * @return  ProxyFilter
     */
    public function filter( $condition_1 )
    {
        return $this->getProxyTarget()->filter( func_get_args() );
    }
    
    /**
     * @param   mixed       $condition_1
     * @return  ProxyFilter
     */
    public function filterOut( $condition_1 )
    {
        return $this->getProxyTarget()->filterOut( func_get_args() );
    }
    
    public function getFilters()
    {
        return $this->getProxyTarget()->getFilters();
    }
    
}
