#include "PakFile.h"

namespace EDUtilities {
	PakFile::PakFile() {
	}

	PakFile::~PakFile() {
		ClearPak();
	}

	PakFile* PakFile::GetInstance() {
		static PakFile pak_file;
		return &pak_file;
	}

	void PakFile::FindAssetName(const char* _raw_name, std::string& _final_name) {
		// TODO - comment out solution version and write your own implementation
		FindAssetNameSolution(_raw_name, _final_name);
	}

	bool PakFile::LoadAsset(const char* _asset_name, char** _buffer, unsigned int& _file_size) {
		// TODO - comment out solution version and write your own implementation
		//return LoadAssetSolution(_asset_name, _buffer, _file_size);

		if ( IsInPak(_asset_name) ) {
			std::string assetName;
			FindAssetName(_asset_name, assetName);
			auto isInPakIt = pak_directory.find(assetName);
			_file_size = isInPakIt->second.file_length;
			*_buffer = new char[_file_size];
			pak_file.seekg(isInPakIt->second.file_offset, std::ios_base::beg);
			pak_file.read(*_buffer, _file_size);
			return true;
		} else {
			return false;
		}
	}

	void PakFile::ClearPak() {
		// TODO - comment out solution version and write your own implementation
		//ClearPakSolution();
		pak_directory.clear();
		if ( pak_file.is_open() ) {
			pak_file.close();
		}
	}

	bool PakFile::IsInPak(const char* _file_name) {
		// TODO - comment out solution version and write your own implementation
		std::string fileName;
		FindAssetName(_file_name, fileName);
		auto isInPakIt = pak_directory.find(fileName);
		return isInPakIt != pak_directory.end();

		// return IsInPakSolution(_file_name);
	}

	void PakFile::LoadDirectory(const char* _pak_file) {
		// TODO - comment out solution version and write your own implementation
		pak_file.open(_pak_file, std::ios_base::binary | std::ios_base::in);

		if ( pak_file.is_open() ) {
			MainHeaderPak pakHeader;
			pak_file.read((char*)&pakHeader, sizeof(MainHeaderPak));

			if ( strncmp(pakHeader.signature, "PACK", 4) != 0 || !(pakHeader.directory_length > 0)) {
				return;
			}

			pak_file.seekg(pakHeader.directory_offset, std::ios_base::beg);
			pak_directory.clear();

			for ( int i = 0; i < pakHeader.directory_length / (int)(sizeof(FileHeaderPak)); i++ ) {
				FileHeaderPak fileHeader;
				pak_file.read((char*)&fileHeader, sizeof(FileHeaderPak));

				std::string fileName = fileHeader.filename;
				pak_directory[fileName] = fileHeader;
			}
		}
		
		//LoadDirectorySolution(_pak_file);
	}

	void PakFile::ExtractAsset(const char* _asset_name) {
		// TODO - comment out solution version and write your own implementation
		//ExtractAssetSolution(_asset_name);
		if ( IsInPak(_asset_name) ) {

			std::ofstream fout;
			fout.open(_asset_name, std::ios_base::binary | std::ios_base::out);
			const unsigned int FixSize = 64000;

			if ( fout.is_open() ) {
				std::string assetName;
				FindAssetName(_asset_name, assetName);
				auto isInPakIt = pak_directory.find(assetName);
				char buffer[FixSize];
				int fileSize = isInPakIt->second.file_length;
				size_t numChunck = fileSize / FixSize;
				size_t leftSize = fileSize % FixSize;
				pak_file.seekg(isInPakIt->second.file_offset, std::ios_base::beg);
				for ( size_t i = 0; i < numChunck; i++ ) {
					pak_file.read(buffer, FixSize);
					fout.write(buffer, FixSize);
				}
				pak_file.read(buffer, leftSize);
				fout.write(buffer, leftSize);
				fout.close();
			}
		}
	}
}
