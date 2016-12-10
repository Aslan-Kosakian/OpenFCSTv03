//---------------------------------------------------------------------------
//
//    FCST: Fuel Cell Simulation Toolbox
//
//    Copyright (C) 2009-13 by Energy Systems Design Laboratory, University of Alberta
//
//    This software is distributed under the MIT License.
//    For more information, see the README file in /doc/LICENSE
//
//    - Class: geometries.h
//    - Description: Geometry definition for several fuel cell elements.
//    - Developers: L. Birkett, P. Dobson, M. Secanell and V. Zingan
//    - $Id: geometries.h 2605 2014-08-15 03:36:44Z secanell $
//
//---------------------------------------------------------------------------


#ifndef dimension
#if deal_II_dimension == 1
#define _1D_
#elif deal_II_dimension == 2
#define _2D_
#elif deal_II_dimension == 3
#define _3D_
#endif
#endif

#ifndef _FUELCELLSHOP__GEOMETRIES_H
#define _FUELCELLSHOP__GEOMETRIES_H

#include <grid/geometry.h>

// STD:
#include <numeric>

namespace FuelCellShop
{
    namespace Geometry
    {
        
        /**
         *  \brief This class imports meshes from dealii supported file types
         *
         *  This class reads a mesh from any of the dealii supported file types. It is the users responsibility
         *  to provide the correct material and boundary ID's. The units if the mesh are in FCST standard units.
         * 
         *  The grid is generated by execution the \ref generate_grid() function. This class contains the 
         *  inmplementation of the pure virtual class method.
         * 
         * \author L. Birkett
         * \author P. Dobson
         * \author M. Secanell
         * \author Valentin N. Zingan: Author of Salome parser dealii::GridIn<dim, spacedim>::read_unv(): Valentin N. Zingan, University of Alberta, (C) 2012.
         * 
         */
        template <int dim>
        class GridExternal
        : public GridBase<dim>
        {
        public:
            /** Default constructor */
            GridExternal(const std::string &name)
            : GridBase<dim>()
            {}
            
            /**
             * PROTOTYPE constructor
             * 
             * \warning For internal use only. Use #create_GridGenerator instead.
             */
            GridExternal()
            {
                    this->get_mapFactory()->insert(std::pair<std::string, GridExternal<dim>* > (this->concrete_name, this) );           
            }
            
            /** 
             * \brief Generate deal.ii triangulation object from a file.
             */
            virtual void generate_grid(Triangulation<dim> &triangulation);
            
            static const std::string concrete_name;
            
        protected:
            
            ///@name Instance Delivery (function)
            //@{
            /**
             * This member function is used to create an object of type micro porous layer
             * 
             * \warning This class MUST be redeclared in every child.
             */
            virtual boost::shared_ptr<FuelCellShop::Geometry::GridBase<dim> > create_replica (const std::string &name)
            {
                return boost::shared_ptr<FuelCellShop::Geometry::GridBase<dim> > (new FuelCellShop::Geometry::GridExternal<dim> (name));
            }   
            //@}
            
            ///@name Instance Delivery (Data member)
            //@{
            /**
             * Create prototype for the layer
             */          
            static GridExternal<dim> const* PROTOTYPE;
            //@}
            
        };
        
        /**
         *  \brief This function generates a square or cubic mesh.
         *
         *  The grid has all boundary_IDs as 0 and material_ID as 1.
         */
        template <int dim>
        class HyperCube
        : public GridBase<dim>
        {
        public:
            /** Default constructor */
            HyperCube(const std::string &name)
            : GridBase<dim>()
            {}
            
            /**
             * PROTOTYPE constructor
             * 
             * \warning For internal use only. Use #create_GridGenerator instead.
             */
            HyperCube()
            {
                    this->get_mapFactory()->insert(std::pair<std::string, HyperCube<dim>* > (this->concrete_name, this) );
            }
            
            /** 
             * \brief Generate deal.ii triangulation object from a file.
             */
            virtual void generate_grid(Triangulation<dim> &triangulation);
            
            static const std::string concrete_name;
            
        protected:
            
            ///@name Instance Delivery (function)
            //@{
            /**
             * This member function is used to create an object of type micro porous layer
             * 
             * \warning This class MUST be redeclared in every child.
             */
            virtual boost::shared_ptr<FuelCellShop::Geometry::GridBase<dim> > create_replica (const std::string &name)
            {
                return boost::shared_ptr<FuelCellShop::Geometry::GridBase<dim> > (new FuelCellShop::Geometry::HyperCube<dim> (name));
            }   
            //@}
            
            ///@name Instance Delivery (Data member)
            //@{
            /**
             * Create prototype for the layer
             */          
            static HyperCube<dim> const* PROTOTYPE;
            //@}
            
        };
        
        /**
         *  \brief This function generates a grid for a cathode including gas diffusion, microporous, and catalyst layers.
         *
         *  The grid contains all material IDs and boundary IDs and default values are shown below
         *  in the \ref generate_grid() function.
         */
        template <int dim>
        class CathodeMPL
        : public GridBase<dim>
        {
        public:
            /**
             * PROTOTYPE constructor
             * 
             * \warning For internal use only. Use #create_GridGenerator instead.
             */
            CathodeMPL()
            : GridBase<dim>()
            {
                    this->get_mapFactory()->insert(std::pair<std::string, CathodeMPL<dim>* > (this->concrete_name, this) );           
            }  
            
            /**This function generates the cathode side of the PEMFC grid including gas diffusion, microporous and catalyst layer.
             *  The grid contains all material IDs and boundary IDs and default values are shown below
             *  Material ID:
             * - Cathode GDL    2
             * - Cathode MPL    3
             * - Cathode CL     4
             * - Membrane       5
             *
             *  Boundary ID:
             * - Cathode GDL/BPP           1
             * - Cathode GDL/Gas Channel   2
             * - Cathode GDL/MPL           9
             * - Cathode MPL/CL            10
             * - Cathode CL/Membrane       4
             * - Anode CL/Membrane         5
             * - Symmetric                 0
             *
             *  Schematic
             *       |         |     |    |         |
             * BID 2 |          |    |    |         |
             *       |         |     |    |         |
             *  	 |   GDL   | MPL | CL |   Mem   |
             * MID   |    2    |  3  | 4  |    5    |
             *       |         |     |    |         |
             * BID   1         9     10   4         5
             *       |         |     |    |         |
             *         Cathode
             *
             *  Lengths:
             * - Cathode GDL       2.0E-2cm
             * - Cathode MPL       2.0E-3cm
             * - Cathode CL        1.0E-3cm
             * - Membrane          2.0E-2cm
             * - Gas Channel       0.1cm
             * - Bipolar Plate Rib 0.1cm
             *
             *  Cells          6 cells tall
             * -                Cathode GDL       4 cells wide
             * -                Cathode MPL       1 cell wide
             * -                Cathode CL        1 cell wide
             * -                Membrane          4 cells wide
             *
             *  Note: to automatically refine the grid after grid creation set the number of initial refeinements parameter
             */
            virtual void generate_grid(Triangulation<dim> &triangulation);
            
            
            
            static const std::string concrete_name;
            
        protected:
            /** Default constructor */
            CathodeMPL(const std::string &name)
            : GridBase<dim>()
            {}
            ///@name Instance Delivery (function)
            //@{
            /**
             * This member function is used to create an object of type micro porous layer
             * 
             * \warning This class MUST be redeclared in every child.
             */
            virtual boost::shared_ptr<FuelCellShop::Geometry::GridBase<dim> > create_replica (const std::string &name)
            {
                return boost::shared_ptr<FuelCellShop::Geometry::GridBase<dim> > (new FuelCellShop::Geometry::CathodeMPL<dim> (name));
            }   
            //@}
            
            ///@name Instance Delivery (Data member)
            //@{
            /**
             * Create prototype for the layer
             */          
            static CathodeMPL<dim> const* PROTOTYPE;
            //@}                
        };        
        
        /**
         *  \brief This function generates a PEMFC grid including a membrane, gas diffusion, microporous, and catalyst layers.
         *
         *  The grid contains all material IDs and boundary IDs and default values are shown below
         *  in the \ref generate_grid() function.
         * 
         *  If you would like to create a cathode, an anode or an MEA without an MPL, simply set the thickness of the layers
         *  you do not want to use to zero. The program will automatically remove that domain from the computational mesh.
         * 
         *  <h3> Default values:</h3>
         *  Material ID:
         * - Cathode GDL    2
         * - Cathode MPL    3
         * - Cathode CL     4
         * - Membrane       5
         * - Anode CL       6
         * - Anode MPL      7
         * - Anode GDL      8
         *
         *  Boundary ID:
         * - Cathode GDL/BPP           1
         * - Cathode GDL/Gas Channel   2
         * - Cathode GDL/MPL           9
         * - Cathode MPL/CL            10
         * - Cathode CL/Membrane       4
         * - Anode CL/Membrane         5
         * - Anode CL/MPL              11
         * - Anode MPL/GDL             12
         * - Anode GDL/BPP             7
         * - Anode GDL/Gas Channel     8
         * - Symmetric                 0
         *
         *  Schematic
         *    |         |     |    |         |    |       |         |
         * BID 2        |     |    |         |    |       |         8
         *    |         |     |    |         |    |       |         |
         *    |   GDL   | MPL | CL |   Mem   | CL |  MPL  |   GDL   |
         * MID|    2    |  3  | 4  |    5    |  6 |   7   |    8    |
         *    |         |     |    |         |    |       |         |
         * BID1         9     10   4         5   11       12        7
         *    |         |     |    |         |    |       |         |
         *      Cathode                            Anode
         *
         *  Lengths:
         * - Cathode + Anode GDL       2.0E-2cm
         * - Cathode + Anode MPL       2.0E-3cm
         * - Cathode + Anode CL        1.0E-3cm
         * - Membrane                  2.0E-2cm
         * - Gas Channel               0.1cm
         * - Bipolar Plate Rib         0.1cm
         *
         *  Cells:
         * - 6 cells tall
         * - Cathode + Anode GDL       4 cells wide
         * - Cathode + Anode MPL       1 cell wide
         * - Cathode + Anode CL        1 cell wide
         * - Membrane                  4 cells wide
         *
         *  Note: to automatically refine the grid after grid creation set the number of initial refinements parameter
         * 
         */
        template <int dim>
        class PemfcMPL
        : public GridBase<dim>
        {
        public:
            /**
             * PROTOTYPE constructor
             * 
             * \warning For internal use only. Use #create_GridGenerator instead.
             */
            PemfcMPL()
            : GridBase<dim> ()
            {
                this->get_mapFactory()->insert(std::pair<std::string, PemfcMPL<dim>* > (this->concrete_name, this) );           
            }            
            /**This function generates a full PEMFC grid including gas diffusion, microporous and catalyst layers.
             *  The grid contains all material IDs and boundary IDs and default values are shown below
             *  Material ID:
             * - Cathode GDL    2
             * - Cathode MPL    3
             * - Cathode CL     4
             * - Membrane       5
             * - Anode CL       6
             * - Anode MPL      7
             * - Anode GDL      8
             *
             *  Boundary ID:
             * - Cathode GDL/BPP           1
             * - Cathode GDL/Gas Channel   2
             * - Cathode GDL/MPL           9
             * - Cathode MPL/CL            10
             * - Cathode CL/Membrane       4
             * - Anode CL/Membrane         5
             * - Anode CL/MPL              11
             * - Anode MPL/GDL             12
             * - Anode GDL/BPP             7
             * - Anode GDL/Gas Channel     8
             * - Symmetric                 0
             *
             *  Schematic:
             *     |         |     |    |         |    |       |         |
             * BID 2         |     |    |         |    |       |         8
             * 	   |         |     |    |         |    |       |         |
             *     |   GDL   | MPL | CL |   Mem   | CL |  MPL  |   GDL   |
             * MID |    2    |  3  | 4  |    5    |  6 |   7   |    8    |
             *     |         |     |    |         |    |       |         |
             * BID 1         9     10   4         5   11       12        7
             *     |         |     |    |         |    |       |         |
             *      Cathode                            Anode
             *
             *  Lengths:
             * - Cathode + Anode GDL       2.0E-2cm
             * - Cathode + Anode MPL       2.0E-3cm
             * - Cathode + Anode CL        1.0E-3cm
             * - Membrane                  2.0E-2cm
             * - Gas Channel               0.1cm
             * - Bipolar Plate Rib         0.1cm
             *
             *  Cells:
             * - 6 cells tall
             * - Cathode + Anode GDL       4 cells wide
             * - Cathode + Anode MPL       1 cell wide
             * - Cathode + Anode CL        1 cell wide
             * - Membrane                  4 cells wide
             *
             *  Note: to automatically refine the grid after grid creation set the number of initial refinements parameter
             */
            virtual void generate_grid(Triangulation<dim> &triangulation);
            
            static const std::string concrete_name;
            
        protected:
            /** Default constructor */
            PemfcMPL(const std::string &name)
            : GridBase<dim> ()
            {}
            ///@name Instance Delivery (function)
            //@{
            /**
             * This member function is used to create an object of type micro porous layer
             * 
             * \warning This class MUST be redeclared in every child.
             */
            virtual boost::shared_ptr<FuelCellShop::Geometry::GridBase<dim> > create_replica (const std::string &name)
            {
                return boost::shared_ptr<FuelCellShop::Geometry::GridBase<dim> > (new FuelCellShop::Geometry::PemfcMPL<dim> (name));
            }   
            //@}
            
            ///@name Instance Delivery (Data member)
            //@{
            /**
             * Create prototype for the layer
             */          
            static PemfcMPL<dim> const* PROTOTYPE;
            //@}                
            
        };
        
        
        /**
         *  \brief This function generates an agglomerate with a core and a thin film subdomain.
         *
         *  The grid contains all material IDs and boundary IDs and default values are shown below
         *  in the \ref generate_grid() function.
         */
        template <int dim>
        class Agglomerate
        : public GridBase<dim>
        {
        public:
            /** Default constructor */
            Agglomerate(const std::string &name)
            : GridBase<dim> ()
            {}
            
            /**
             * PROTOTYPE constructor
             * 
             * \warning For internal use only. Use #create_GridGenerator instead.
             */
            Agglomerate()
            : GridBase<dim> ()
            {
                this->get_mapFactory()->insert(std::pair<std::string, Agglomerate<dim>* > (this->concrete_name, this) );           
            }
            
            /**This function generates an agglomerate grid with a core and thin film subdomain
             *  The grid contains all material IDs and boundary IDs and default values are shown below
             *
             *  Material ID:
             *  Agglomerate Core = 1
             *  Thin Film = 2
             */
            virtual void generate_grid(Triangulation<dim> &triangulation);
            
            void reset_material_ids(Triangulation<dim> &triangulation) const;
            
            static const std::string concrete_name;
            
        protected:
            
            ///@name Instance Delivery (function)
            //@{
            /**
             * This member function is used to create an object of type micro porous layer
             * 
             * \warning This class MUST be redeclared in every child.
             */
            virtual boost::shared_ptr<FuelCellShop::Geometry::GridBase<dim> > create_replica (const std::string &name)
            {
                return boost::shared_ptr<FuelCellShop::Geometry::GridBase<dim> > (new FuelCellShop::Geometry::Agglomerate<dim> (name));
            }   
            //@}
            
            ///@name Instance Delivery (Data member)
            //@{
            /**
             * Create prototype for the layer
             */          
            static Agglomerate<dim> const* PROTOTYPE;
            //@}                
            
        };
        /**
         *  \brief This class treates a test mesh with one element
         *   
         *  \author M. Secanell
         * 
         */
        template <int dim>
        class GridTest
        : public GridBase<dim>
        {
        public:
            /**
             * PROTOTYPE constructor
             * 
             * \warning For internal use only. Use #create_GridGenerator instead.
             */
            GridTest()
            {
                this->get_mapFactory()->insert(std::pair<std::string, GridTest<dim>* > (this->concrete_name, this) );           
            }
            
            
            /** Default constructor */
            GridTest(const std::string &name)
            : GridBase<dim>()
            {}
            
            
            /** 
             * \brief Generate deal.ii triangulation object from a file.
             */
            virtual void generate_grid(Triangulation<dim> &triangulation);
            
            static const std::string concrete_name;
            
        protected:
            
            ///@name Instance Delivery (function)
            //@{
            /**
             * This member function is used to create an object of type micro porous layer
             * 
             * \warning This class MUST be redeclared in every child.
             */
            virtual boost::shared_ptr<FuelCellShop::Geometry::GridBase<dim> > create_replica (const std::string &name)
            {
                return boost::shared_ptr<FuelCellShop::Geometry::GridBase<dim> > (new FuelCellShop::Geometry::GridTest<dim> (name));
            }   
            //@}
            
            ///@name Instance Delivery (Data member)
            //@{
            /**
             * Create prototype for the layer
             */          
            static GridTest<dim> const* PROTOTYPE;
            //@}
            
        };
    }
}
#endif