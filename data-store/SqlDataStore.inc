<?php
/**
 * Copyright Skinit, Inc.
 */

abstract class SqlDataStore extends OrmDataStore implements IKeylessAccess, ISchematic
{
    
    /**
     * Given the provided SQL-specific data type, return the corresponding schematic data type.  The schematic data
     * types are available as ISchematic::TYPE_* constants.
     * 
     * @param   string  $sqlType
     * @return  string
     */
    protected abstract function mapDataType( $sqlType );
    
    /**
     * Execute the provided query and return an iterator over the results.  If values are provided, bind them to the
     * replacement tokens ('?') in the query.
     * 
     * @param   string      $query
     * @param   array       $values
     * @return  Iterator
     */
    protected abstract function query( $query, $values = array() );

    /**
     * Return the first result from the provided query.
     * 
     * @param   string      $query
     * @param   array       $values
     * @return  array 
     */
    protected function queryOne( $query, $values = array() )
    {
        foreach( $this->query( $query, $values ) as $record )
        {
            return $record;
        }
    }
    
    /**
     * Return the first value from the first result of the provided query.
     * 
     * @param   string          $query
     * @param   array           $values
     * @return  string|bool|int
     */
    protected function queryScalar( $query, $values )
    {
        $record = $this->queryOne( $query, $values );
        return array_shift( $record );
    }
    
    /**
     * Return the first value from every record in the result of the provided query.
     * 
     * @param   string      $query
     * @param   array       $values
     * @return  array 
     */
    protected function queryColumn( $query, $values = array() )
    {
        $result = array();
        foreach( $this->query( $query, $values ) as $record )
        {
            $result[] = array_shift( $record );
        }
        return $result;
    }

    /**************************************************************************
     * OrmDataStore implementation
     **************************************************************************/
    
    public function fetch( $relation, $key )
    {
        $query = "select * from \"$relation\"";
        $filter = array();
        $values = array();
        foreach( $key as $name => $value )
        {
            $filter[] = "\"$name\" = ?";
            $values[] = $value;
        }
        $query .= $filter
          ? " where " . implode( " and ", $filter )
          : "";
        return $this->queryOne( $query, $values );
    }
    
    /**************************************************************************
     * IKeylessAccess implementation
     **************************************************************************/

    public function findAll( $relation )
    {
        $query = "select * from \"$relation\"";
        return $this->query( $query, array() );
    }
    
    /**************************************************************************
     * ISchematic implementation 
     **************************************************************************/
        
    public function getRelations()
    {
        // grab the tables from the information schema
        $query  = "select table_name from information_schema.tables";
        $query .= " where schema_name = ?";
        $values = array( $this->getName() );
        
        // build SchemaRelation instances describing the tables
        $results = array();
        foreach( $this->queryColumn( $query, $values ) as $tableName )
        {
            $results[] = new SchemaRelation( $this, $tableName );
        }
        
        // return the relations
        return $results;
    }

    public function getRelationAttributes( $relationName )
    {
        // grab the columns and types from the information schema
        $query  = "select column_name, data_type from information_schema.columns";
        $query .= " where schema_name = ? and table_name = ?";
        $values = array( $this->getName(), $relationName );
        
        // build SchemaAttribute instances describing the columns
        $results = array();
        foreach( $this->query( $query, $values ) as $record )
        {
            $name = $record['name'];
            $dataType = $this->mapDataType( $record['data_type'] );
            $results[] = new SchemaAttribute( $this, $relationName, $name, $dataType );
        }

        // return the attributes
        return $results;
    }
    
    public function getRelationKeyAttributes( $relationName )
    {
        // grab the PK columns and types from the information schema
    }
    
    public function getRelationships( $relationName )
    {
        // grab the FK constraints from the information schema
        $query  = "select referenced_table_name, column_name, referenced_column_name, ordinal_position";
        $query .= " from information_schema.key_column_usage";
        $query .= " where table_schema = ? and table_name = ?";
        $query .= " order by constraint_name, ordinal_position";
        $values = array( $this->getName(), $relationName );
        
        // build SchemaRelationship instances describing the FK constraints
        $results = array();
        $fromKey = array();
        $toKey = array();
        foreach( $this->query( $query, $values ) as $record )
        {
            // if we are at ordinal position 0, we have a new relationship
            $ordinal = $record['ordinal_position'];
            if( $ordinal == 0 )
            {
                // before we move to the new relationship, generate the previous one
                if( $fromKey && $toKey )
                {
                    $fromKey = new SchemaKey( $fromKey );
                    $toKey = new SchemaKey( $toKey );
                    $results[] = new SchemaRelationship( $fromKey, $toKey );
                    $fromKey = array();
                    $toKey = array();
                }
                
                $attributeName = $record['column_name'];
                $matches = array_filter( $this->getRelationAttributes( $relationName ), function($attribute) use($attributeName) {
                    return $attribute->getName() == $attributeName;
                } );
                $fromKey[] = array_shift( $matches );
                
                $referencedRelation = $record['referenced_table_name'];
                $attributeName = $record['referenced_column_name'];
                $matches = array_filter( $this->getRelationAttributes( $referencedRelation ), function($attribute) use($attributeName) {
                    return $attribute->getName() == $attributeName;
                } );
                $toKey[] = array_shift( $matches );
            }
        }
        
        // create final relationship from leftover data
        if( $fromKey && $toKey )
        {
            $fromKey = new SchemaKey( $fromKey );
            $toKey = new SchemaKey( $toKey );
            $results[] = new SchemaRelationship( $fromKey, $toKey );
        }
                
        // return the relationship
        return $results;
    }
        
}