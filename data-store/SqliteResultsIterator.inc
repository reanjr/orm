<?php
/**
 * Copyright Skinit, Inc.
 */

class SqliteResultsIterator implements Iterator
{
    protected $index = 0;
    protected $currentResult = array();
    protected $results = null;
    
    /**
     * Set the Sqlite resultset to iterate over.
     * 
     * @param   SQLite3Result   $results
     */
    public function __construct( SQLite3Result $results )
    {
        $this->results = $results;
    }
    
    public function current()
    {
        return $this->currentResult;
    }
    
    public function key()
    {
        return $this->index;
    }
    
    public function next()
    {
        $this->currentResult = $this->results->fetchArray();
        $this->index++;
    }
    
    public function rewind()
    {
        // no op
    }
    
    public function valid()
    {
        return (bool)$this->currentResult;
    }
    
}