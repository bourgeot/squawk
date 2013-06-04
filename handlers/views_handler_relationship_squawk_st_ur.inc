<?php
class views_handler_relationship_squawk_st_ur views_handler_relationship {
   function init(&$view, &$options) {
    parent::init($view, $options);

	/**
   * Called to implement a relationship in a query.
   
   *here is where it is called in the view inc file
   
     $data['node']['term_node_tid'] = array(
    'title' => t('Taxonomy terms on node'),
    'help' => t('Relate nodes to taxonomy terms, specifiying which vocabulary or vocabularies to use. This relationship will cause duplicated records if there are multiple terms.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship_node_term_data',
      'label' => t('term'),
      'base' => 'taxonomy_term_data',
    ),
    'field' => array(
      'title' => t('All taxonomy terms'),
      'help' => t('Display all taxonomy terms associated with a node from specified vocabularies.'),
      'handler' => 'views_handler_field_term_node_tid',
      'no group by' => TRUE,
    ),
  );

   */
  function query() {
    $this->ensure_my_table();

    $def = $this->definition;
    $def['table'] = 'user_relationship_types';

    if (!array_filter($this->options['vocabularies'])) {
      $ur = $this->query->add_table('user_relationships', $this->relationship);
      $def['left_table'] = $ur;
      $def['left_field'] = 'rtid';
      $def['field'] = 'rtid';
      $def['type'] = 'INNER';
    }
    /*else {
      // If vocabularies are supplied join a subselect instead
      $def['left_table'] = $this->table_alias;
      $def['left_field'] = 'nid';
      $def['field'] = 'nid';
      $def['type'] = empty($this->options['required']) ? 'LEFT' : 'INNER';

      $query = db_select('taxonomy_term_data', 'td');
      $query->addJoin($def['type'], 'taxonomy_vocabulary', 'tv', 'td.vid = tv.vid');
      $query->addJoin($def['type'], 'taxonomy_index', 'tn', 'tn.tid = td.tid');
      $query->condition('tv.machine_name', array_filter($this->options['vocabularies']));
      $query->addTag('term_access');
      $query->fields('td');
      $query->fields('tn', array('nid'));
      $def['table formula'] = $query;
    }*/

    $join = new views_join();

    $join->definition = $def;
    $join->construct();
    $join->adjusted = TRUE;

    // use a short alias for this:
    $alias = $def['table'] . '_' . $this->table;

    $this->alias = $this->query->add_relationship($alias, $join, 'user_relationship_types', $this->relationship);
  }

};