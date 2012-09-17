<?php
/**
 * Copyright Skinit, Inc.
 */

class FilterParser
{
    const EVENT_BEGIN = "begin";
    const EVENT_CONJUNCTION = "conjunction";
    const EVENT_DISJUNCTION = "disjunction";
    const EVENT_GROUP = "group";
    const EVENT_UNGROUP = "ungroup";
    const EVENT_COMPARISON = "comparison";
    const EVENT_INVERSION = "inversion";

    const COMPARE_EQUALS = "=";
    
    protected $handlers = array();
    protected $filters = array();
    protected $data;
    
    /**
     * Set the filters to parse.
     * 
     * @param   array   $filters
     */
    public function __construct( array $filters )
    {
        foreach( $filters as $filter )
        {
            $this->addFilter( $filter );
        }
    }
    
    /**
     * Create a new FilterParse instance with the provided filters and return it.
     * 
     * @param   array           $filters
     * @return  FilterParser
     */
    public static function create( array $filters )
    {
        return new FilterParser( $filters );
    }
    
    /**
     * Parse the filters and generate events.
     */
    public function parse()
    {
        // begin parsing events
        $this->triggerEvent( self::EVENT_BEGIN );
        
        // generate events for the filters
        /* @var $filter Filter */
        $filters = $this->filters;
        while( $filter = array_shift( $filters  ) )
        {
            $filter->parse( $this );
            
            // add a combinator if there are more filters
            if( $filters )
            {
                $this->triggerEvent( self::EVENT_CONJUNCTION );
            }
        }
    }
    
    /**
     * Add a filter to parse.  All filters will be combined as conjunctions (the AND operator in SQL).
     * 
     * @param   Filter  $filter
     */
    public function addFilter( Filter $filter )
    {
        $this->filters[] = $filter;
    }
    
    /**
     * Return all the filters to be parsed.
     * 
     * @return  array
     */
    public function getFilters()
    {
        return $this->filters;
    }
    
    /**
     * Set extra data that will be provided to parse event handlers.  Return this instance for chaining.
     * 
     * @param   mixed           $data
     * @return  FilterParser
     */
    public function setData( &$data )
    {
        $this->data = &$data;
        return $this;
    }
    
    /**
     * Register a parse event handler.  The event should be one of the FilterParser::EVENT_* constants.  Only one
     * handler may be registered for each event.  If a second handler is set for an event, it will override the
     * existing handler.  Return this instance for chaining.
     * 
     * @param   string          $event
     * @param   callback        $handler
     * @return  FilterParser
     */
    public function setEventHandler( $event, $handler )
    {
        // validate handler
        if( !is_callable( $handler ) )
        {
            throw new Exception( "handler must be callable" );
        }
        
        // set the handler for the event
        $this->handlers[$event] = $handler;
        
        // return this instance
        return $this;
    }
    
    /**
     * Trigger the specified event.  If a handler is set for the event, it will be called.  The event should be one of
     * the FilterParser::EVENT_* constants.
     * 
     * @param   string  $event
     * @param   mixed   $argument_1
     */
    public function triggerEvent( $event, $argument_1 = null )
    {
        $args = func_get_args();
        $args[0] = $this->data;
        array_unshift( $args, $event );
        
        // call the event handler
        if( isset($this->handlers[$event]) )
        {
            call_user_func_array( $this->handlers[$event], $args );
        }
    }
    
}