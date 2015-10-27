#include "PythonModule.h"
#include "InternalOutput.h"

namespace EDUtilities {
	void PythonModule::Initialize() {
		// internal output must be created before python inits
		InternalOutput::GetReference();

		// tells python where to look for its 'lib' files
		Py_NoSiteFlag = 1;
		Py_SetPythonHome(L"../Python/.");
		Py_Initialize();

		// tell python where to search for modules
		std::wstring new_path = Py_GetPath();
		new_path += L";../Scripts";
		PySys_SetPath(new_path.c_str());
	}

	void PythonModule::Shutdown() {
		Py_Finalize();
	}

	void PythonModule::ReloadModule() {
		ReloadModuleSolution();
	}

	PythonModule::PythonModule(string _name) {
		LoadModule(_name);
	}

	bool PythonModule::LoadModule(string _filename) {
		// TODO - comment this out and write your own solution
		bool success = false;
		//success = LoadModuleSolution(_filename);
		
		module = PyImport_ImportModule(_filename.c_str());

		if ( module ) {
			if ( PyErr_Occurred() ) {
				PyErr_Print();
			}
			Py_INCREF(module);
			dictionary = PyModule_GetDict(module);
			module_name = PyModule_GetName(module);
			success = true;
		}

		return success;
	}

	void PythonModule::UnloadModule() {
		// TODO - comment this out and write your own solution
		//UnloadModuleSolution();
		if ( module ) {
			Py_DECREF(module);
			module = nullptr;
			dictionary = nullptr;
			module_name.clear();
		}
	}

	void PythonModule::CallFunction(string _name) {
		// TODO - comment this out and write your own solution
		//CallFunctionSolution(_name);

		PyObject *pyFunc = PyDict_GetItemString(dictionary, _name.c_str());
		if ( pyFunc ) {
			PyObject *result = PyObject_CallObject(pyFunc, NULL);
			if ( result ) {

			}
		}
	}

	void PythonModule::SetValue(string _name, int _value) {
		// TODO - comment this out and write your own solution
		//SetValueSolution(_name, _value);
		PyObject *pyValue = PyLong_FromLong(_value);
		PyDict_SetItemString(dictionary, _name.c_str(), pyValue);
	}

	void PythonModule::SetValue(string _name, float _value) {
		// TODO - comment this out and write your own solution
		//SetValueSolution(_name, _value);
		PyObject *pyValue = PyLong_FromDouble(_value);
		PyDict_SetItemString(dictionary, _name.c_str(), pyValue);
	}

	void PythonModule::SetValue(string _name, string _value) {
		// TODO - comment this out and write your own solution
		//SetValueSolution(_name, _value);
		PyObject *pyValue = PyUnicode_FromString(_value.c_str());
		PyDict_SetItemString(dictionary, _name.c_str(), pyValue);
	}

	void PythonModule::SetValue(string _name, bool _value) {
		// TODO - comment this out and write your own solution
		//SetValueSolution(_name, _value);
		PyObject *pyValue = PyBool_FromLong(_value);
		PyDict_SetItemString(dictionary, _name.c_str(), pyValue);
	}

	void PythonModule::GetValue(string _name, int& _value) {
		// TODO - comment this out and write your own solution
		//GetValueSolution(_name, _value);
		PyObject *pyValue = PyDict_GetItemString(dictionary, _name.c_str());
		if ( pyValue ) {
			_value = (int)PyLong_AsLong(pyValue);
		}
	}

	void PythonModule::GetValue(string _name, float& _value) {
		// TODO - comment this out and write your own solution
		//GetValueSolution(_name, _value);
		PyObject *pyValue = PyDict_GetItemString(dictionary, _name.c_str());
		if ( pyValue ) {
			_value = (float)PyFloat_AsDouble(pyValue);
		}
	}

	void PythonModule::GetValue(string _name, string& _value) {
		// TODO - comment this out and write your own solution
		//GetValueSolution(_name, _value);
		PyObject *pyValue = PyDict_GetItemString(dictionary, _name.c_str());
		if ( pyValue ) {
			_value = string(PyUnicode_AsUTF8(pyValue));
		}
	}

	void PythonModule::GetValue(string _name, bool& _value) {
		// TODO - comment this out and write your own solution
		//GetValueSolution(_name, _value);
		PyObject *pyValue = PyDict_GetItemString(dictionary, _name.c_str());
		if ( pyValue ) {
			if ( PyObject_IsTrue(pyValue) == 1 ) {
				_value = true;
			} else {
				_value = false;
			}
		}
	}
}
