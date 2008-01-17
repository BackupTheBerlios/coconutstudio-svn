//
// C++ Interface: undo_redo
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef UNDO_REDO_H
#define UNDO_REDO_H


#include "rstring.h"
#include <list>

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class UndoRedo {
protected:

	struct CommandBase {
	
		virtual void call()=0;	
		virtual ~CommandBase() {}
	};
	
	template<class T>
	struct Command0 : public CommandBase {
	
		typedef void (T::*Method)();
		T * instance;
		Method method;
		virtual void call() {
			(instance->*method)();
		}
	};

	template<class T,class P1>
	struct Command1 : public CommandBase {
		
		typedef void (T::*Method)(P1);
		T* instance;
		Method method;
		P1 p1;
		virtual void call() {
			(instance->*method)(p1);
		}
		Command1(T* p_instance,Method p_method,P1 p_p1) { instance=p_instance; method=p_method; p1 = p_p1;  }
	};
	
	
	template<class T,class M,class P1>
	Command1<T,P1>* command( T *p_instance, M p_method, P1 p1) {
	
		return new Command1<T,P1>(p_instance,p_method,p1);
	}
	
	/**/

	template<class T,class P1,class P2>
	struct Command2 : public CommandBase {
		
		typedef void (T::*Method)(P1,P2);
		T* instance;
		Method method;
		P1 p1; P2 p2;
		virtual void call() {
			(instance->*method)(p1,p2);
		}
		Command2(T* p_instance,Method p_method,P1 p_p1,P2 p_p2) { instance=p_instance; method=p_method; p1 = p_p1; p2=p_p2; }
	};
	
	
	template<class T,class M,class P1,class P2>
	Command2<T,P1,P2>* command( T *p_instance, M p_method, P1 p1, P2 p2) {
	
		return new Command2<T,P1,P2>(p_instance,p_method,p1,p2);
	}
	
	/**/
		
	template<class T,class P1,class P2, class P3>
	struct Command3 : public CommandBase {
		
		typedef void (T::*Method)(P1,P2,P3);
		T* instance;
		Method method;
		P1 p1; P2 p2; P3 p3;
		virtual void call() {
			(instance->*method)(p1,p2,p3);
		}
		Command3(T* p_instance,Method p_method,P1 p_p1,P2 p_p2,P3 p_p3) { instance=p_instance; method=p_method; p1 = p_p1; p2=p_p2; p3 = p_p3; }
	};
	
	
	template<class T,class M,class P1,class P2, class P3>
	Command3<T,P1,P2,P3>* command( T *p_instance, M p_method, P1 p1, P2 p2,P3 p3) {
	
		return new Command3<T,P1,P2,P3>(p_instance,p_method,p1,p2,p3);
	}
	
	/**/
		
	template<class T,class P1,class P2, class P3, class P4>
	struct Command4 : public CommandBase {
		
		typedef void (T::*Method)(P1,P2,P3,P4);
		T* instance;
		Method method;
		P1 p1; P2 p2; P3 p3; P4 p4;
		virtual void call() {
			(instance->*method)(p1,p2,p3,p4);
		}
		Command4(T* p_instance,Method p_method,P1 p_p1,P2 p_p2,P3 p_p3,P4 p_p4) { instance=p_instance; method=p_method; p1 = p_p1; p2=p_p2; p3 = p_p3; p4=p_p4; }
	};
	
	
	template<class T,class M,class P1,class P2, class P3, class P4>
	Command4<T,P1,P2,P3,P4>* command( T *p_instance, M p_method, P1 p1, P2 p2,P3 p3, P4 p4) {
	
		return new Command4<T,P1,P2,P3,P4>(p_instance,p_method,p1,p2,p3,p4);
	}
	
	/**/
	
	template<class T,class P1,class P2, class P3, class P4, class P5>
	struct Command5 : public CommandBase {
		
		typedef void (T::*Method)(P1,P2,P3,P4,P5);
		T* instance;
		Method method;
		P1 p1; P2 p2; P3 p3; P4 p4; P5 p5;
		virtual void call() {
			(instance->*method)(p1,p2,p3,p4,p5);
		}
		Command5(T* p_instance,Method p_method,P1 p_p1,P2 p_p2,P3 p_p3,P4 p_p4, P5 p_p5) { instance=p_instance; method=p_method; p1 = p_p1; p2=p_p2; p3 = p_p3; p4=p_p4; p5 = p_p5; }
	};
	
	
	template<class T,class M,class P1,class P2, class P3, class P4, class P5>
	Command5<T,P1,P2,P3,P4,P5>* command( T *p_instance, M p_method, P1 p1, P2 p2,P3 p3, P4 p4, P5 p5) {
	
		return new Command5<T,P1,P2,P3,P4,P5>(p_instance,p_method,p1,p2,p3,p4,p5);
	}


	/*****/
	
	class Action {
		CommandBase *undo_method;
		CommandBase *redo_method;
	public:
	
		void undo() { if (undo_method) undo_method->call(); }
		void redo() { if (redo_method) redo_method->call(); }
		Action(CommandBase *p_undo_method,CommandBase *p_redo_method) { undo_method=p_undo_method; redo_method=p_redo_method; }
		
		~Action() { if (undo_method) delete undo_method; if (redo_method) delete redo_method; }
	};

	void begin_group(String p_name);
	void end_group();
	void add_action(String p_description,CommandBase *p_do_method, CommandBase *p_undo_method);

private:

	struct ActionGroup {
	
		std::list<Action*> action_list;
		String name;
			
	};

public:
	UndoRedo();
	~UndoRedo();

};

#endif
