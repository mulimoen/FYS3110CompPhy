#include "catch.hpp"

#include "lattice.hpp"


TEST_CASE( "SANITIY" )
{
  CHECK( 1 == 1 );
  CHECK( 2 == 3-1 );
}


TEST_CASE( "Setting matrix, indexing and changing elements" , "[lattice]")
{
  int max_x = 2;
  int max_y = 4;
  Lattice lattice(max_x, max_y, init::ones);
  
  SECTION( "Check successful init") {
    for (int ii=0; ii<max_y; ii++){
      for (int jj=0; jj<max_x; jj++){
        REQUIRE( lattice(ii,jj) == 1 );
      }
    }
  }
  
  SECTION( "Change element by index") {
    lattice(0, 1) = -5;
    CHECK( lattice(0,1) == -5 );
  }
  
  SECTION( "Indexing periodic boundary" ) {
    lattice(1, 0) = -5;
    lattice(1, max_y - 1) = 3; // last y-element
    
    SECTION( "Wrapping around x-axis" ){
      CHECK(lattice(1+max_x, 0) == -5);
    }
    SECTION( "Wrapping around y-axis" ){
      CHECK(lattice(1, max_y) == -5);
    }
    SECTION( "Wrapping around negative values" ){
      CHECK(lattice(1, -1) == 3); // wrap around negative
    }
  }
  
  
}

