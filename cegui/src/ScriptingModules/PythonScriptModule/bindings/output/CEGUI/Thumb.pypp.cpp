// This file has been generated by Py++.

#include "boost/python.hpp"
#include "python_CEGUI.h"
#include "Thumb.pypp.hpp"

namespace bp = boost::python;

struct Thumb_wrapper : CEGUI::Thumb, bp::wrapper< CEGUI::Thumb > {

    Thumb_wrapper(::CEGUI::String const & type, ::CEGUI::String const & name )
    : CEGUI::Thumb( boost::ref(type), boost::ref(name) )
      , bp::wrapper< CEGUI::Thumb >(){
        // constructor
    
    }

    virtual void beginInitialisation(  ) {
        if( bp::override func_beginInitialisation = this->get_override( "beginInitialisation" ) )
            func_beginInitialisation(  );
        else{
            this->CEGUI::Window::beginInitialisation(  );
        }
    }
    
    void default_beginInitialisation(  ) {
        CEGUI::Window::beginInitialisation( );
    }

    virtual void cloneChildWidgetsTo( ::CEGUI::Window & target ) const  {
        if( bp::override func_cloneChildWidgetsTo = this->get_override( "cloneChildWidgetsTo" ) )
            func_cloneChildWidgetsTo( boost::ref(target) );
        else{
            this->CEGUI::Window::cloneChildWidgetsTo( boost::ref(target) );
        }
    }
    
    void default_cloneChildWidgetsTo( ::CEGUI::Window & target ) const  {
        CEGUI::Window::cloneChildWidgetsTo( boost::ref(target) );
    }

    virtual void clonePropertiesTo( ::CEGUI::Window & target ) const  {
        if( bp::override func_clonePropertiesTo = this->get_override( "clonePropertiesTo" ) )
            func_clonePropertiesTo( boost::ref(target) );
        else{
            this->CEGUI::Window::clonePropertiesTo( boost::ref(target) );
        }
    }
    
    void default_clonePropertiesTo( ::CEGUI::Window & target ) const  {
        CEGUI::Window::clonePropertiesTo( boost::ref(target) );
    }

    virtual void destroy(  ) {
        if( bp::override func_destroy = this->get_override( "destroy" ) )
            func_destroy(  );
        else{
            this->CEGUI::Window::destroy(  );
        }
    }
    
    void default_destroy(  ) {
        CEGUI::Window::destroy( );
    }

    virtual void endInitialisation(  ) {
        if( bp::override func_endInitialisation = this->get_override( "endInitialisation" ) )
            func_endInitialisation(  );
        else{
            this->CEGUI::Window::endInitialisation(  );
        }
    }
    
    void default_endInitialisation(  ) {
        CEGUI::Window::endInitialisation( );
    }

    virtual void fireEvent( ::CEGUI::String const & name, ::CEGUI::EventArgs & args, ::CEGUI::String const & eventNamespace="" ) {
        if( bp::override func_fireEvent = this->get_override( "fireEvent" ) )
            func_fireEvent( boost::ref(name), boost::ref(args), boost::ref(eventNamespace) );
        else{
            this->CEGUI::EventSet::fireEvent( boost::ref(name), boost::ref(args), boost::ref(eventNamespace) );
        }
    }
    
    void default_fireEvent( ::CEGUI::String const & name, ::CEGUI::EventArgs & args, ::CEGUI::String const & eventNamespace="" ) {
        CEGUI::EventSet::fireEvent( boost::ref(name), boost::ref(args), boost::ref(eventNamespace) );
    }

    virtual void getRenderingContext_impl( ::CEGUI::RenderingContext & ctx ) const  {
        if( bp::override func_getRenderingContext_impl = this->get_override( "getRenderingContext_impl" ) )
            func_getRenderingContext_impl( boost::ref(ctx) );
        else{
            this->CEGUI::Window::getRenderingContext_impl( boost::ref(ctx) );
        }
    }
    
    void default_getRenderingContext_impl( ::CEGUI::RenderingContext & ctx ) const  {
        CEGUI::Window::getRenderingContext_impl( boost::ref(ctx) );
    }

    virtual ::CEGUI::Rectf getUnclippedInnerRect_impl(  ) const  {
        if( bp::override func_getUnclippedInnerRect_impl = this->get_override( "getUnclippedInnerRect_impl" ) )
            return func_getUnclippedInnerRect_impl(  );
        else{
            return this->CEGUI::Window::getUnclippedInnerRect_impl(  );
        }
    }
    
    ::CEGUI::Rectf default_getUnclippedInnerRect_impl(  ) const  {
        return CEGUI::Window::getUnclippedInnerRect_impl( );
    }

    virtual void initialiseComponents(  ) {
        if( bp::override func_initialiseComponents = this->get_override( "initialiseComponents" ) )
            func_initialiseComponents(  );
        else{
            this->CEGUI::Window::initialiseComponents(  );
        }
    }
    
    void default_initialiseComponents(  ) {
        CEGUI::Window::initialiseComponents( );
    }

    virtual bool isHit( ::CEGUI::Vector2f const & position, bool const allow_disabled=false ) const  {
        if( bp::override func_isHit = this->get_override( "isHit" ) )
            return func_isHit( boost::ref(position), allow_disabled );
        else{
            return this->CEGUI::Window::isHit( boost::ref(position), allow_disabled );
        }
    }
    
    bool default_isHit( ::CEGUI::Vector2f const & position, bool const allow_disabled=false ) const  {
        return CEGUI::Window::isHit( boost::ref(position), allow_disabled );
    }

    virtual void performChildWindowLayout(  ) {
        if( bp::override func_performChildWindowLayout = this->get_override( "performChildWindowLayout" ) )
            func_performChildWindowLayout(  );
        else{
            this->CEGUI::Window::performChildWindowLayout(  );
        }
    }
    
    void default_performChildWindowLayout(  ) {
        CEGUI::Window::performChildWindowLayout( );
    }

    virtual void setLookNFeel( ::CEGUI::String const & look ) {
        if( bp::override func_setLookNFeel = this->get_override( "setLookNFeel" ) )
            func_setLookNFeel( boost::ref(look) );
        else{
            this->CEGUI::Window::setLookNFeel( boost::ref(look) );
        }
    }
    
    void default_setLookNFeel( ::CEGUI::String const & look ) {
        CEGUI::Window::setLookNFeel( boost::ref(look) );
    }

    virtual void setMargin( ::CEGUI::UBox const & margin ) {
        if( bp::override func_setMargin = this->get_override( "setMargin" ) )
            func_setMargin( boost::ref(margin) );
        else{
            this->CEGUI::Window::setMargin( boost::ref(margin) );
        }
    }
    
    void default_setMargin( ::CEGUI::UBox const & margin ) {
        CEGUI::Window::setMargin( boost::ref(margin) );
    }

    virtual ::CEGUI::RefCounted< CEGUI::BoundSlot > subscribeScriptedEvent( ::CEGUI::String const & name, ::CEGUI::String const & subscriber_name ) {
        if( bp::override func_subscribeScriptedEvent = this->get_override( "subscribeScriptedEvent" ) )
            return func_subscribeScriptedEvent( boost::ref(name), boost::ref(subscriber_name) );
        else{
            return this->CEGUI::EventSet::subscribeScriptedEvent( boost::ref(name), boost::ref(subscriber_name) );
        }
    }
    
    ::CEGUI::RefCounted< CEGUI::BoundSlot > default_subscribeScriptedEvent( ::CEGUI::String const & name, ::CEGUI::String const & subscriber_name ) {
        return CEGUI::EventSet::subscribeScriptedEvent( boost::ref(name), boost::ref(subscriber_name) );
    }

    virtual ::CEGUI::RefCounted< CEGUI::BoundSlot > subscribeScriptedEvent( ::CEGUI::String const & name, unsigned int group, ::CEGUI::String const & subscriber_name ) {
        if( bp::override func_subscribeScriptedEvent = this->get_override( "subscribeScriptedEvent" ) )
            return func_subscribeScriptedEvent( boost::ref(name), group, boost::ref(subscriber_name) );
        else{
            return this->CEGUI::EventSet::subscribeScriptedEvent( boost::ref(name), group, boost::ref(subscriber_name) );
        }
    }
    
    ::CEGUI::RefCounted< CEGUI::BoundSlot > default_subscribeScriptedEvent( ::CEGUI::String const & name, unsigned int group, ::CEGUI::String const & subscriber_name ) {
        return CEGUI::EventSet::subscribeScriptedEvent( boost::ref(name), group, boost::ref(subscriber_name) );
    }

    virtual void update( float elapsed ) {
        if( bp::override func_update = this->get_override( "update" ) )
            func_update( elapsed );
        else{
            this->CEGUI::Window::update( elapsed );
        }
    }
    
    void default_update( float elapsed ) {
        CEGUI::Window::update( elapsed );
    }

    virtual void writeXMLToStream( ::CEGUI::XMLSerializer & xml_stream ) const  {
        if( bp::override func_writeXMLToStream = this->get_override( "writeXMLToStream" ) )
            func_writeXMLToStream( boost::ref(xml_stream) );
        else{
            this->CEGUI::Window::writeXMLToStream( boost::ref(xml_stream) );
        }
    }
    
    void default_writeXMLToStream( ::CEGUI::XMLSerializer & xml_stream ) const  {
        CEGUI::Window::writeXMLToStream( boost::ref(xml_stream) );
    }

};

void register_Thumb_class(){

    { //::CEGUI::Thumb
        typedef bp::class_< Thumb_wrapper, bp::bases< CEGUI::PushButton >, boost::noncopyable > Thumb_exposer_t;
        Thumb_exposer_t Thumb_exposer = Thumb_exposer_t( "Thumb", "*!\n\
        \n\
           Base class for Thumb widget.\n\
           \n\
           The thumb widget is used to compose other widgets (like sliders and scroll bars).  You would\n\
           not normally need to use this widget directly unless you are making a new widget of some type.\n\
        *\n", bp::init< CEGUI::String const &, CEGUI::String const & >(( bp::arg("type"), bp::arg("name") ), "*************************************************************************\n\
           Construction  Destruction\n\
        *************************************************************************\n\
        *!\n\
        \n\
           Constructor for Thumb objects\n\
        *\n") );
        bp::scope Thumb_scope( Thumb_exposer );
        { //::CEGUI::Thumb::getHorzRange
        
            typedef ::std::pair< float, float > ( ::CEGUI::Thumb::*getHorzRange_function_type )(  ) const;
            
            Thumb_exposer.def( 
                "getHorzRange"
                , getHorzRange_function_type( &::CEGUI::Thumb::getHorzRange )
                , "*!\n\
               \n\
                  Return a std.pair that describes the current range set for the horizontal movement.\n\
            \n\
               @return\n\
                  a std.pair describing the current horizontal range.  The first element is the minimum value,\n\
                  the second element is the maximum value.\n\
               *\n" );
        
        }
        { //::CEGUI::Thumb::getVertRange
        
            typedef ::std::pair< float, float > ( ::CEGUI::Thumb::*getVertRange_function_type )(  ) const;
            
            Thumb_exposer.def( 
                "getVertRange"
                , getVertRange_function_type( &::CEGUI::Thumb::getVertRange )
                , "*!\n\
               \n\
                  Return a std.pair that describes the current range set for the vertical movement.\n\
            \n\
               @return\n\
                  a std.pair describing the current vertical range.  The first element is the minimum value,\n\
                  the second element is the maximum value.\n\
               *\n" );
        
        }
        { //::CEGUI::Thumb::isHorzFree
        
            typedef bool ( ::CEGUI::Thumb::*isHorzFree_function_type )(  ) const;
            
            Thumb_exposer.def( 
                "isHorzFree"
                , isHorzFree_function_type( &::CEGUI::Thumb::isHorzFree )
                , "*!\n\
               \n\
                  return whether the thumb is movable on the horizontal axis.\n\
            \n\
               @return\n\
                  true if the thumb is movable along the horizontal axis.\n\
                  false if the thumb is fixed on the horizontal axis.\n\
               *\n" );
        
        }
        { //::CEGUI::Thumb::isHotTracked
        
            typedef bool ( ::CEGUI::Thumb::*isHotTracked_function_type )(  ) const;
            
            Thumb_exposer.def( 
                "isHotTracked"
                , isHotTracked_function_type( &::CEGUI::Thumb::isHotTracked )
                , "*************************************************************************\n\
                  Accessor Functions\n\
               ************************************************************************* \n\
               *!\n\
               \n\
                  return whether hot-tracking is enabled or not.\n\
            \n\
               @return\n\
                  true if hot-tracking is enabled.  false if hot-tracking is disabled.\n\
               *\n" );
        
        }
        { //::CEGUI::Thumb::isVertFree
        
            typedef bool ( ::CEGUI::Thumb::*isVertFree_function_type )(  ) const;
            
            Thumb_exposer.def( 
                "isVertFree"
                , isVertFree_function_type( &::CEGUI::Thumb::isVertFree )
                , "*!\n\
               \n\
                  return whether the thumb is movable on the vertical axis.\n\
            \n\
               @return\n\
                  true if the thumb is movable along the vertical axis.\n\
                  false if the thumb is fixed on the vertical axis.\n\
               *\n" );
        
        }
        { //::CEGUI::Thumb::setHorzFree
        
            typedef void ( ::CEGUI::Thumb::*setHorzFree_function_type )( bool ) ;
            
            Thumb_exposer.def( 
                "setHorzFree"
                , setHorzFree_function_type( &::CEGUI::Thumb::setHorzFree )
                , ( bp::arg("setting") )
                , "*!\n\
               \n\
                  set whether thumb is movable on the horizontal axis.\n\
            \n\
               @param setting\n\
                  true to allow movement of thumb along the horizontal axis.  false to fix thumb on the\
                  horizontal axis.\n\
            \n\
               @return\n\
                  nothing.\n\
               *\n" );
        
        }
        { //::CEGUI::Thumb::setHorzRange
        
            typedef void ( ::CEGUI::Thumb::*setHorzRange_function_type )( float,float ) ;
            
            Thumb_exposer.def( 
                "setHorzRange"
                , setHorzRange_function_type( &::CEGUI::Thumb::setHorzRange )
                , ( bp::arg("min"), bp::arg("max") )
                , "*!\n\
               \n\
                  set the movement range of the thumb for the horizontal axis.\n\
            \n\
                  The values specified here are relative to the parent window for the thumb, and are specified\
                  in whichever\n\
                  metrics mode is active for the widget.\n\
            \n\
               @param min\n\
                  the minimum setting for the thumb on the horizontal axis.\n\
            \n\
               @param max\n\
                  the maximum setting for the thumb on the horizontal axis.\n\
            \n\
               @return\n\
                  Nothing.\n\
               *\n" );
        
        }
        { //::CEGUI::Thumb::setHotTracked
        
            typedef void ( ::CEGUI::Thumb::*setHotTracked_function_type )( bool ) ;
            
            Thumb_exposer.def( 
                "setHotTracked"
                , setHotTracked_function_type( &::CEGUI::Thumb::setHotTracked )
                , ( bp::arg("setting") )
                , "*************************************************************************\n\
                  Manipulator Functions\n\
               *************************************************************************\n\
               *!\n\
               \n\
                  set whether the thumb uses hot-tracking.\n\
            \n\
               @param setting\n\
                  true to enable hot-tracking.  false to disable hot-tracking.\n\
            \n\
               @return\n\
                  Nothing.\n\
               *\n" );
        
        }
        { //::CEGUI::Thumb::setVertFree
        
            typedef void ( ::CEGUI::Thumb::*setVertFree_function_type )( bool ) ;
            
            Thumb_exposer.def( 
                "setVertFree"
                , setVertFree_function_type( &::CEGUI::Thumb::setVertFree )
                , ( bp::arg("setting") )
                , "*!\n\
               \n\
                  set whether thumb is movable on the vertical axis.\n\
            \n\
               @param setting\n\
                  true to allow movement of thumb along the vertical axis.  false to fix thumb on the vertical\
                  axis.\n\
            \n\
               @return\n\
                  nothing.\n\
               *\n" );
        
        }
        { //::CEGUI::Thumb::setVertRange
        
            typedef void ( ::CEGUI::Thumb::*setVertRange_function_type )( float,float ) ;
            
            Thumb_exposer.def( 
                "setVertRange"
                , setVertRange_function_type( &::CEGUI::Thumb::setVertRange )
                , ( bp::arg("min"), bp::arg("max") )
                , "*!\n\
               \n\
                  set the movement range of the thumb for the vertical axis.\n\
            \n\
                  The values specified here are relative to the parent window for the thumb, and are specified\
                  in whichever\n\
                  metrics mode is active for the widget.\n\
            \n\
               @param min\n\
                  the minimum setting for the thumb on the vertical axis.\n\
            \n\
               @param max\n\
                  the maximum setting for the thumb on the vertical axis.\n\
            \n\
               @return\n\
                  Nothing.\n\
               *\n" );
        
        }
        Thumb_exposer.add_static_property( "EventThumbPositionChanged"
                        , bp::make_getter( &CEGUI::Thumb::EventThumbPositionChanged
                                , bp::return_value_policy< bp::return_by_value >() ) );
        Thumb_exposer.add_static_property( "EventThumbTrackEnded"
                        , bp::make_getter( &CEGUI::Thumb::EventThumbTrackEnded
                                , bp::return_value_policy< bp::return_by_value >() ) );
        Thumb_exposer.add_static_property( "EventThumbTrackStarted"
                        , bp::make_getter( &CEGUI::Thumb::EventThumbTrackStarted
                                , bp::return_value_policy< bp::return_by_value >() ) );
        { //::CEGUI::Window::beginInitialisation
        
            typedef void ( ::CEGUI::Window::*beginInitialisation_function_type )(  ) ;
            typedef void ( Thumb_wrapper::*default_beginInitialisation_function_type )(  ) ;
            
            Thumb_exposer.def( 
                "beginInitialisation"
                , beginInitialisation_function_type(&::CEGUI::Window::beginInitialisation)
                , default_beginInitialisation_function_type(&Thumb_wrapper::default_beginInitialisation) );
        
        }
        { //::CEGUI::Window::cloneChildWidgetsTo
        
            typedef void ( ::CEGUI::Window::*cloneChildWidgetsTo_function_type )( ::CEGUI::Window & ) const;
            typedef void ( Thumb_wrapper::*default_cloneChildWidgetsTo_function_type )( ::CEGUI::Window & ) const;
            
            Thumb_exposer.def( 
                "cloneChildWidgetsTo"
                , cloneChildWidgetsTo_function_type(&::CEGUI::Window::cloneChildWidgetsTo)
                , default_cloneChildWidgetsTo_function_type(&Thumb_wrapper::default_cloneChildWidgetsTo)
                , ( bp::arg("target") ) );
        
        }
        { //::CEGUI::Window::clonePropertiesTo
        
            typedef void ( ::CEGUI::Window::*clonePropertiesTo_function_type )( ::CEGUI::Window & ) const;
            typedef void ( Thumb_wrapper::*default_clonePropertiesTo_function_type )( ::CEGUI::Window & ) const;
            
            Thumb_exposer.def( 
                "clonePropertiesTo"
                , clonePropertiesTo_function_type(&::CEGUI::Window::clonePropertiesTo)
                , default_clonePropertiesTo_function_type(&Thumb_wrapper::default_clonePropertiesTo)
                , ( bp::arg("target") ) );
        
        }
        { //::CEGUI::Window::destroy
        
            typedef void ( ::CEGUI::Window::*destroy_function_type )(  ) ;
            typedef void ( Thumb_wrapper::*default_destroy_function_type )(  ) ;
            
            Thumb_exposer.def( 
                "destroy"
                , destroy_function_type(&::CEGUI::Window::destroy)
                , default_destroy_function_type(&Thumb_wrapper::default_destroy) );
        
        }
        { //::CEGUI::Window::endInitialisation
        
            typedef void ( ::CEGUI::Window::*endInitialisation_function_type )(  ) ;
            typedef void ( Thumb_wrapper::*default_endInitialisation_function_type )(  ) ;
            
            Thumb_exposer.def( 
                "endInitialisation"
                , endInitialisation_function_type(&::CEGUI::Window::endInitialisation)
                , default_endInitialisation_function_type(&Thumb_wrapper::default_endInitialisation) );
        
        }
        { //::CEGUI::EventSet::fireEvent
        
            typedef void ( ::CEGUI::EventSet::*fireEvent_function_type )( ::CEGUI::String const &,::CEGUI::EventArgs &,::CEGUI::String const & ) ;
            typedef void ( Thumb_wrapper::*default_fireEvent_function_type )( ::CEGUI::String const &,::CEGUI::EventArgs &,::CEGUI::String const & ) ;
            
            Thumb_exposer.def( 
                "fireEvent"
                , fireEvent_function_type(&::CEGUI::EventSet::fireEvent)
                , default_fireEvent_function_type(&Thumb_wrapper::default_fireEvent)
                , ( bp::arg("name"), bp::arg("args"), bp::arg("eventNamespace")="" ) );
        
        }
        { //::CEGUI::Window::getRenderedStringParser
        
            typedef ::CEGUI::RenderedStringParser & ( ::CEGUI::Window::*getRenderedStringParser_function_type )(  ) const;
            
            Thumb_exposer.def( 
                "getRenderedStringParser"
                , getRenderedStringParser_function_type(&::CEGUI::Window::getRenderedStringParser)
                , bp::return_value_policy< bp::reference_existing_object >() );
        
        }
        { //::CEGUI::Window::getRenderingContext_impl
        
            typedef void ( ::CEGUI::Window::*getRenderingContext_impl_function_type )( ::CEGUI::RenderingContext & ) const;
            typedef void ( Thumb_wrapper::*default_getRenderingContext_impl_function_type )( ::CEGUI::RenderingContext & ) const;
            
            Thumb_exposer.def( 
                "getRenderingContext_impl"
                , getRenderingContext_impl_function_type(&::CEGUI::Window::getRenderingContext_impl)
                , default_getRenderingContext_impl_function_type(&Thumb_wrapper::default_getRenderingContext_impl)
                , ( bp::arg("ctx") ) );
        
        }
        { //::CEGUI::Window::getUnclippedInnerRect_impl
        
            typedef ::CEGUI::Rectf ( ::CEGUI::Window::*getUnclippedInnerRect_impl_function_type )(  ) const;
            typedef ::CEGUI::Rectf ( Thumb_wrapper::*default_getUnclippedInnerRect_impl_function_type )(  ) const;
            
            Thumb_exposer.def( 
                "getUnclippedInnerRect_impl"
                , getUnclippedInnerRect_impl_function_type(&::CEGUI::Window::getUnclippedInnerRect_impl)
                , default_getUnclippedInnerRect_impl_function_type(&Thumb_wrapper::default_getUnclippedInnerRect_impl) );
        
        }
        { //::CEGUI::Window::initialiseComponents
        
            typedef void ( ::CEGUI::Window::*initialiseComponents_function_type )(  ) ;
            typedef void ( Thumb_wrapper::*default_initialiseComponents_function_type )(  ) ;
            
            Thumb_exposer.def( 
                "initialiseComponents"
                , initialiseComponents_function_type(&::CEGUI::Window::initialiseComponents)
                , default_initialiseComponents_function_type(&Thumb_wrapper::default_initialiseComponents) );
        
        }
        { //::CEGUI::Window::isHit
        
            typedef bool ( ::CEGUI::Window::*isHit_function_type )( ::CEGUI::Vector2f const &,bool const ) const;
            typedef bool ( Thumb_wrapper::*default_isHit_function_type )( ::CEGUI::Vector2f const &,bool const ) const;
            
            Thumb_exposer.def( 
                "isHit"
                , isHit_function_type(&::CEGUI::Window::isHit)
                , default_isHit_function_type(&Thumb_wrapper::default_isHit)
                , ( bp::arg("position"), bp::arg("allow_disabled")=(bool const)(false) ) );
        
        }
        { //::CEGUI::Window::performChildWindowLayout
        
            typedef void ( ::CEGUI::Window::*performChildWindowLayout_function_type )(  ) ;
            typedef void ( Thumb_wrapper::*default_performChildWindowLayout_function_type )(  ) ;
            
            Thumb_exposer.def( 
                "performChildWindowLayout"
                , performChildWindowLayout_function_type(&::CEGUI::Window::performChildWindowLayout)
                , default_performChildWindowLayout_function_type(&Thumb_wrapper::default_performChildWindowLayout) );
        
        }
        { //::CEGUI::Window::setLookNFeel
        
            typedef void ( ::CEGUI::Window::*setLookNFeel_function_type )( ::CEGUI::String const & ) ;
            typedef void ( Thumb_wrapper::*default_setLookNFeel_function_type )( ::CEGUI::String const & ) ;
            
            Thumb_exposer.def( 
                "setLookNFeel"
                , setLookNFeel_function_type(&::CEGUI::Window::setLookNFeel)
                , default_setLookNFeel_function_type(&Thumb_wrapper::default_setLookNFeel)
                , ( bp::arg("look") ) );
        
        }
        { //::CEGUI::Window::setMargin
        
            typedef void ( ::CEGUI::Window::*setMargin_function_type )( ::CEGUI::UBox const & ) ;
            typedef void ( Thumb_wrapper::*default_setMargin_function_type )( ::CEGUI::UBox const & ) ;
            
            Thumb_exposer.def( 
                "setMargin"
                , setMargin_function_type(&::CEGUI::Window::setMargin)
                , default_setMargin_function_type(&Thumb_wrapper::default_setMargin)
                , ( bp::arg("margin") ) );
        
        }
        { //::CEGUI::EventSet::subscribeScriptedEvent
        
            typedef ::CEGUI::RefCounted< CEGUI::BoundSlot > ( ::CEGUI::EventSet::*subscribeScriptedEvent_function_type )( ::CEGUI::String const &,::CEGUI::String const & ) ;
            typedef ::CEGUI::RefCounted< CEGUI::BoundSlot > ( Thumb_wrapper::*default_subscribeScriptedEvent_function_type )( ::CEGUI::String const &,::CEGUI::String const & ) ;
            
            Thumb_exposer.def( 
                "subscribeScriptedEvent"
                , subscribeScriptedEvent_function_type(&::CEGUI::EventSet::subscribeScriptedEvent)
                , default_subscribeScriptedEvent_function_type(&Thumb_wrapper::default_subscribeScriptedEvent)
                , ( bp::arg("name"), bp::arg("subscriber_name") ) );
        
        }
        { //::CEGUI::EventSet::subscribeScriptedEvent
        
            typedef ::CEGUI::RefCounted< CEGUI::BoundSlot > ( ::CEGUI::EventSet::*subscribeScriptedEvent_function_type )( ::CEGUI::String const &,unsigned int,::CEGUI::String const & ) ;
            typedef ::CEGUI::RefCounted< CEGUI::BoundSlot > ( Thumb_wrapper::*default_subscribeScriptedEvent_function_type )( ::CEGUI::String const &,unsigned int,::CEGUI::String const & ) ;
            
            Thumb_exposer.def( 
                "subscribeScriptedEvent"
                , subscribeScriptedEvent_function_type(&::CEGUI::EventSet::subscribeScriptedEvent)
                , default_subscribeScriptedEvent_function_type(&Thumb_wrapper::default_subscribeScriptedEvent)
                , ( bp::arg("name"), bp::arg("group"), bp::arg("subscriber_name") ) );
        
        }
        { //::CEGUI::Window::update
        
            typedef void ( ::CEGUI::Window::*update_function_type )( float ) ;
            typedef void ( Thumb_wrapper::*default_update_function_type )( float ) ;
            
            Thumb_exposer.def( 
                "update"
                , update_function_type(&::CEGUI::Window::update)
                , default_update_function_type(&Thumb_wrapper::default_update)
                , ( bp::arg("elapsed") ) );
        
        }
        { //::CEGUI::Window::writeXMLToStream
        
            typedef void ( ::CEGUI::Window::*writeXMLToStream_function_type )( ::CEGUI::XMLSerializer & ) const;
            typedef void ( Thumb_wrapper::*default_writeXMLToStream_function_type )( ::CEGUI::XMLSerializer & ) const;
            
            Thumb_exposer.def( 
                "writeXMLToStream"
                , writeXMLToStream_function_type(&::CEGUI::Window::writeXMLToStream)
                , default_writeXMLToStream_function_type(&Thumb_wrapper::default_writeXMLToStream)
                , ( bp::arg("xml_stream") ) );
        
        }
    }

}
