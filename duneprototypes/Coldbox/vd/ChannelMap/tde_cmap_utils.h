#ifndef __TDE_CMAP_UTILS__
#define __TDE_CMAP_UTILS__

#include <iostream>
#include <set>
#include <vector>
#include <utility>
#include <tuple> 

namespace dune { namespace tde {
    const short ch_per_kel = 32;
    const short ch_per_amc = 64;

    typedef struct kel_connector
    {
      kel_connector()
      {
	_id       = -1;
	_view     = -1;
	_reverse  = false;
	_first_view_ch = 0;
      }
    
      kel_connector( int id, int view, bool reverse,
		     int first_view_ch, short nch )
      {
	_id   = id;
	_view = view;
	_reverse = reverse;
	_first_view_ch = first_view_ch;
	_nch           = nch; // number of this view channels per this connector
      }

      void print() const{
	std::cout<<"KEL "<<_id<<" : view "<<_view
		 <<" reverse order "<<_reverse
		 <<" first view ch for this connector "<<_first_view_ch
		 <<" total ch in this view "<<_nch<<std::endl;
      }
    
      bool operator < (const kel_connector &other) const
      {
	if( _id != other._id ) return _id < other._id;
	return _view < other._view;
      }
     
      int   _id;
      int   _view;
      bool  _reverse;
      int   _first_view_ch;
      short _nch; 
    } kel_connector;

    typedef std::set<kel_connector> connectors;
  
    //
    typedef struct crp_connectors
    {
    
      crp_connectors( int crp ){
	_crp = crp;
      }

      void add_connector( int id, int view, bool reverse,
			  int first_view_ch, short nch = ch_per_kel ){
	_kels.insert( kel_connector( id, view, reverse, first_view_ch, nch) );
      }

      // get number connector attributed to different readout planes
      std::vector<kel_connector> get_connector_views( int kel_id ){
	std::vector<kel_connector> conn_views;
	// just do a brute force loop to find all view for this connector
	for( const auto c: _kels ){
	  if( c._id == kel_id ){
	    conn_views.push_back( c );
	  }
	}
	//std::cout<<"For KEL "<<kel_id
	//<<" found "<<conn_views.size()
	//<<" entries\n";
	return conn_views;
      }

    
      int _crp;
      connectors _kels;
    } crp_connectors;
  
    //
    typedef struct crate
    {
      crate(int id, int cards){
	_id    = id;
	_cards = cards;
      }

      //
      void add_crp_connection( int icrp, int fslot, //slot to add or starting ...
			       const std::vector<int> &kel_ids )
      {
	auto nconn = kel_ids.size();
	if( nconn % 2 ){
	  std::cout<<"number of connectors is not even\n";
	  return;
	}

	if( fslot < (int)_crp_conn.size() &&  !_crp_conn.empty() ){
	  std::cout<<"cannot overwrite the existing connection\n";
	  return;
	}
      
	int islot = fslot;
	for( size_t i=0;i<kel_ids.size();i+=2 ){
	  int kel_1 = kel_ids[i];
	  int kel_2 = kel_ids[i+1];
	
	  if( islot >= _cards  ){
	    std::cout<<"slot "<<islot<<" does not exist\n";
	    break;
	  }
	  _crp_conn.emplace_back(islot, icrp, kel_1, kel_2 );
	  islot++;
	}
      }
    
      //
      int _id;
      int _cards;
      std::vector< std::tuple< int, int, int, int> > _crp_conn;
    } crate;
  
  }//tde
}//dune


#endif
