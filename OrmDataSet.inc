<?php
/**
 * Copyright Skinit, Inc.
 */

class OrmDataSet implements Iterator, IFilterable
{
    protected $relation = null;
    protected $filters = array();
    
    /**
     * Set the primary relation for the data set.
     * 
     * @param   string  $relationName
     */
    public function __construct( $relationName )
    {
        $this->setPrimaryRelation( $relationName );
    }
    
    /**
     * Return a data set for the specified relation.  This data set will generate OrmModel instances for the tuples
     * in the relation.
     * 
     * @return  OrmDataSet
     */
    public static function get( $relationName )
    {
        return new OrmDataSet( $relationName );
    }
    
    /**
     * Return the name of the relation for the data set.  This is the relation for which models will be generated when
     * iterating over the data set.
     * 
     * @return  string
     */
    public function getRelation()
    {
        return $this->relation;
    }
    
    /**
     * Set the relation for the data set.  This is the relation for which models will be generated when iterating over
     * the data set.
     * 
     *   $ormDataSet->setPrimaryRelation( "foo" );
     *   foreach( $ormDataSet as $m )
     *     assert( $m instanceof FooModel );
     * 
     * @param   string  $relationName
     */
    public function setRelation( $relationName )
    {
        // ensure we have a model for the relation; otherwise we don't know what to do with it
        if( !class_exists( ucfirst($relationName) . "Model" ) )
        {
            throw new OrmSchemaException( "relation '$relationName' has no known model" );
        }
        
        // set the relation
        $this->relation = $relationName;
    }
    
    /**
     * Return the name of the model class used for generated instances when iterating.
     * 
     * @return  string
     */
    public function getModelClass()
    {
        return ucfirst( Orm::formatLogicalName( $this->getRelation() ) ) . "Model";
    }
    
    /**
     * Evaluate the data set.  Once evaluated, no further modifications can be made to the data set.  This includes
     * adding additional filters or beginning a starting over with a new iteration.  The data set may only be evaluated
     * once.  Trying to evaluate a second time results in an exception.
     */
    public function evaluate()
    {
        // grab the appropriate data store by checking the model class
        $modelClass = $this->getModelClass();
        $dataStoreName = $modelClass::getDataStoreName();
        Orm::getDataStore( $dataStoreName );
    }
    
    
    /**************************************************************************
     * Iterator implementation
     **************************************************************************/
    
    public function current()
    {
        
    }
    
    public function key()
    {
        // the iterator must currently be set to a valid result for the key to be valid
        if( $this->valid() )
        {
            return $this->key;
        }
    }
    
    public function next()
    {
        $this->key++;
    }
    
    public function rewind()
    {
        // rewind is called first during iteration, so we evaluate at this step
        $this->evaluate();
        $this->key = 0;
    }
    
    public function valid()
    {
        return false;
    }
    
    
    /**************************************************************************
     * IFilterable implementation
     **************************************************************************/
    
    public function filter( $condition_1 )
    {
        $filter = Filter::createProxy( $this )->addTests( func_get_args() );
        $this->filters[] = $filter;
        return $filter;
    }
    
    public function filterOut( $condition_1 )
    {
        $filter = Filter::createProxy( $this )->addTests( func_get_args() )->invert();
        $this->filters[] = $filter;
        return $filter;
    }
    
    public function getFilters()
    {
        return $this->filters;
    }
    
}