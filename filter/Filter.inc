<?php
/**
 * Copyright Skinit, Inc.
 */

class Filter
{
    const OP_DEFAULT = " AND ";
    const OP_ALL = " AND ";
    const OP_ANY = " OR ";
    
    protected $inverted = false;
    protected $conditions = array();
    protected $filters = array();
    protected $op = Filter::OP_DEFAULT;
    
    /**
     * Set the tests for the filter.  Any number of tests may be provided.  By default, all tests must pass for the
     * filter to pass.  This can be controlled with the Filter->invert() method and the Filter->any() and Filter->all()
     * methods.
     * 
     * The format of tests is flexible.  Tests may be Conditions or Filters and may be passed in one of several ways.
     * Examples:
     *  - new Filter( array( "email", "foo@example.com" ) );                    // 2-tuple key/value pair
     *  - new Filter( array( "name" => "foo" ) );                               // set of key/value pairs
     *  - new Filter( array( "foo_id" => new ConditionNotNull() ) );            // key with Condition value
     *  - new Filter( new Condition( "created", "2012-01-01", "<" ) );          // Condition instance
     *  - new Filter( new Filter( array( "foo", "bar" ) ) );                    // Filter instance
     *  - new Filter( array("name"=>"foo"), new ConditionNotNull("foo_id") );   // mix and match any number of times
     *  - new Filter( "foo_id", new ConditionNotNull() );                       // special single condition form
     * 
     * In all forms of the tests passed as arrays, if the key is an integer, the value may be any valid test or array of
     * tests.  If the key is not an integer, then the value should not be an array(†).  If the value is a Condition
     * instance and was added with a non-integer key, the Condition's name will be overridden with the key during
     * processing.  All forms of tests may be mixed and matched as multiple arguments with the exception of the single
     * condition form, which must always have exactly two arguments, one of which is a string, the other a scalar value
     * or a Condition instance.
     * 
     * † this will currently work, but the behavior should not be relied upon.
     * 
     * @param   mixed   $test_1
     */
    public function __construct( $test_1 )
    {
        $this->addTests( func_get_args() );
    }
    
    /**
     * Create a new proxy filter and set the target IFilterable for the filter.  A proxy filter can be used by an
     * IFilterable instance to provided better support for filter chaining.  The proxy filter provides not only methods
     * to modify the filter instance, but also to add additional filters to the proxy filter target object.
     * 
     * @param   IFilterable $target
     * @return  ProxyFilter
     */
    public static function createProxy( IFilterable $target )
    {
        return new ProxyFilter( $target );
    }
    
    /**
     * Add tests to this filter.  The arguments are the same as for the constructor.  Return this instance for chaining.
     * 
     * @param   mixed   $test_1
     * @return  Filter
     */
    public function addTests( $test_1 )
    {
        $args = func_get_args();
        
        // if called in single condition form, strip the arguments and set a condition
        if( count($args) == 2 && is_string( $args[0] ) )
        {
            $this->addCondition( ($args[1] instanceof Condition)
              ? $args[1]->setName($args[0])
              : new Condition( $args[0], $args[1] ) );
            $args = array();
        }
        
        // walk arguments and add to the list of tests
        $instance = $this;
        array_walk_recursive( $args, function($value, $key) use($instance) {
            // unnamed test must be Condition  or Filter instance
            if( is_int($key) && !($value instanceof Condition || $value instanceof Filter) )
            {
               throw new Exception( "unnamed test must be a Condition or Filter instance ($key:$value)" ); 
            }
            
            // override name for named conditions
            if( !is_int($key) && $value instanceof Condition )
            {
                $value->setName( $key );
            }
            
            // filter test must be keyed by int
            if( !is_int($key) && $value instanceof Filter )
            {
                throw new Exception( "unexpected use of non-integer key with Filter test" );
            }
            
            // add a new test
            $test = ($value instanceof Condition || $value instanceof Filter)
              ? $value
              : new Condition( $key, $value );
            ($test instanceof Condition) ? $instance->addCondition( $test ) : $instance->addFilter( $test );
        } );
        
        // return this instance
        return $this;
    }
    
    /**
     * Set the op for this filter.  This controls how conditions interact when there are more than one.  This should be
     * one of the Filter::OP_* constants.
     * 
     * @param   string  $op
     */
    public function setOp( $op )
    {
        $this->op = $op;
    }
    
    /**
     * Set the filter op to "all".  This means the filter will pass if "all" of the conditions pass.
     */
    public function all()
    {
        $this->setOp( self::OP_ALL );
    }
    
    /**
     * Set the filter op to "any".  This means the filter will pass if "any" of the conditions pass.
     */
    public function any()
    {
        $this->setOp( self::OP_ANY );
    }
    
    /**
     * Invert the filter.  An inverted filter passes when the conditions are NOT met.  Calling this method again will
     * result in the filter returning to its original state.
     */
    public function invert()
    {
        $this->inverted = !$this->inverted;
    }
    
    /**
     * Return true if the filter is currently inverted.  Otherwise, return false.
     * 
     * @return  bool
     */
    public function inverted()
    {
        return $this->inverted;
    }
    
    /**
     * Add a condition to the current set of conditions.  Return this instance for chaining.
     * 
     * @param   Condition   $condition
     * @return  Filter
     */
    public function addCondition( Condition $condition )
    {
        $this->conditions[] = $condition;
        return $this;
    }
    
    /**
     * Add an array of conditions to the current set of conditions.  Return this instance for chaining.
     * 
     * @param   array   $conditions
     * @return  Filter
     */
    public function addConditions( array $conditions )
    {
        $this->conditions = array_merge( $this->conditions, $conditions );
    }
    
    /**
     * Return list of conditions defined for this filter.
     * 
     * @return  array
     */
    public function getConditions()
    {
        return $this->conditions;
    }
    
    /**
     * Add a sub-filter to the current filter.  Return this instance for chaining.
     * 
     * @param   Filter  $filter
     * @return  Filter
     */
    public function addFilter( Filter $filter )
    {
        $this->filters[] = $filter;
        return $this;
    }
    
    /**
     * Add an array of sub-filters to the current filter.  Return this instance for chaining.
     * 
     * @param   array   $filters
     * @return  Filter
     */
    public function addFilters( array $filters )
    {
        $this->filters = array_merge( $this->filters, $filters );
    }
    
    /**
     * Return list of sub-filters defined for this filter.
     * 
     * @return  array
     */
    public function getFilters()
    {
        return $this->filters;
    }
    
    /**
     * Use the provided parser to generate parse events for this filter.
     * 
     * @param   FilterParser    $parser
     */
    public function parse( FilterParser $parser )
    {
        // handle inversion if we have one
        if( $this->inverted() )
        {
            $parser->triggerEvent( FilterParser::EVENT_INVERSION );
        }
        
        // group filter tests
        $parser->triggerEvent( FilterParser::EVENT_GROUP );
        
        $tests = array_merge( $this->getConditions(), $this->getFilters() );
        while( $test = array_shift( $tests ) )
        {
            // add a comparison event for conditions
            if( $test instanceof Condition )
            {
                $parser->triggerEvent( FilterParser::EVENT_COMPARISON, $test->getName(), $test->getValue(), $test->getOp() );
            }
            
            // pass along control for sub filters
            elseif( $test instanceof Filter )
            {
                $test->parse( $parser );
            }
            
            // anything else is a problem
            else
            {
                throw new OrmFilterParseException( "tests must be Condition or Filter instances" );
            }
            
            // add a combinator if there are more tests
            if( $tests )
            {
                switch( $this->op )
                {
                    case self::OP_ALL:
                        $parser->triggerEvent( FilterParser::EVENT_CONJUNCTION );
                        break;
                    case self::OP_ANY:
                        $parser->triggerEvent( FilterParser::EVENT_DISJUNCTION );
                        break;
                    default:
                        throw new OrmFilterParseException( "unknown filter op '$op'" );
                }
            }
        }
        
        // close filter tests group
        $parser->triggerEvent( FilterParser::EVENT_UNGROUP );
    }
    
}