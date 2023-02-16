#include "test.hpp"

class FileTest : public ::testing::Test{
	protected:
		void SetUp() override {}
		void TearDown() override {
			_allocator.~LinearAllocator();
			free(_mem);
		}

		void* _mem = malloc(MEMORY_MO);
		Raindrop::Core::Memory::LinearAllocator _allocator{MEMORY_MO, _mem};
		Raindrop::Core::Filesystem::File _file{_allocator, "res/TXT_document.txt"};
};

TEST_F(FileTest, info){
	EXPECT_TRUE(_file.exist());

	Raindrop::Core::String ext = _file.extention(_allocator);
	Raindrop::Core::String name = _file.name(_allocator);

	EXPECT_EQ(strcmp(name.str(), "TXT_document.txt"), 0);
	EXPECT_EQ(strcmp(ext.str(), ".txt"), 0);
	EXPECT_EQ(_file.size(), 32);
}

class TXTDocumentReadTest : public ::testing::Test{
	protected:
		void SetUp() override {}
		void TearDown() override {
			_allocator.~LinearAllocator();
			free(_mem);
		}

		void* _mem = malloc(MEMORY_MO);
		Raindrop::Core::Memory::LinearAllocator _allocator{MEMORY_MO, _mem};
		Raindrop::Core::Filesystem::File _file{_allocator, "res/TXT_document.txt"};
		Raindrop::Core::Filesystem::Filestream _stream{_file, Raindrop::Core::Filesystem::Filestream::OPEN_BEGIN | Raindrop::Core::Filesystem::Filestream::OPEN_READ};
};

TEST_F(TXTDocumentReadTest, init){
	EXPECT_TRUE(_stream.isOpen());
	EXPECT_EQ(_stream.getPos(), 0);
}

TEST_F(TXTDocumentReadTest, read_1_char){
	EXPECT_EQ(_stream.readChar(), 'h');
}

TEST_F(TXTDocumentReadTest, read_string){
	Raindrop::Core::String string = _stream.readString(_allocator, 13);
	// EXPECT_TRUE(string == "hello world !");
}

TEST_F(TXTDocumentReadTest, set_pos){
	_stream.setPos(6);
	EXPECT_EQ(_stream.getPos(), 6);
}

TEST_F(TXTDocumentReadTest, set_pos_end){
	_stream.setPos(0, _stream.POS_END);
	EXPECT_EQ(_stream.getPos(), 32);
}

TEST_F(TXTDocumentReadTest, end_of_file){
	_stream.setPos(0, _stream.POS_END);

	bool hitEnd = false;
	_stream.readChar(&hitEnd);

	EXPECT_TRUE(hitEnd);
}

class FilebufferTest : public ::testing::Test{
	protected:
		void SetUp() override {}
		void TearDown() override {
			_allocator.clear();
			_allocator.~LinearAllocator();
			free(_mem);
		}

		void* _mem = malloc(MEMORY_MO);
		Raindrop::Core::Memory::LinearAllocator _allocator{MEMORY_MO, _mem};
		Raindrop::Core::Filesystem::File _file{_allocator, "res/TXT_document.txt"};
		Raindrop::Core::Filesystem::Filebuffer _buffer{_allocator, _file};
};

TEST_F(FilebufferTest, open){
	EXPECT_EQ(_buffer[0], 'h');
	EXPECT_EQ(_buffer[12], '!');
}

class CSVReaderTest : public ::testing::Test{
	protected:
		void SetUp() override {}
		void TearDown() override {
			_allocator.clear();
			// _csv.~CSVReader();
			// _buffer.~Filebuffer();
			// _file.~File();
			// _allocator.~LinearAllocator();
		}

		~CSVReaderTest(){
			// free(_mem);
		}

		void* _mem = malloc(MEMORY_MO);
		Raindrop::Core::Memory::LinearAllocator _allocator{MEMORY_MO, _mem};
		Raindrop::Core::Filesystem::File _file{_allocator, "res/CSV_document.csv"};
		Raindrop::Core::Filesystem::Filebuffer _buffer{_allocator, _file};
		Raindrop::Core::IO::CSVReader _csv{_allocator, _buffer};
};

TEST_F(CSVReaderTest, init){
	EXPECT_EQ(_csv.columns(), 6);
	EXPECT_EQ(_csv.rows(), 3);

	EXPECT_TRUE(_csv(0, 0) == "1");
	EXPECT_TRUE(_csv(0, 5) == "6");
	EXPECT_TRUE(_csv(1, 0) == "a");
	EXPECT_TRUE(_csv(1, 3) == "d");
	EXPECT_TRUE(_csv(1, 5) == "");
	EXPECT_TRUE(_csv(2, 0) == "word1");
	EXPECT_TRUE(_csv(2, 4) == "word5");
}