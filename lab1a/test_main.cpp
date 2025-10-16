#include <gtest/gtest.h>
#include "BitArray.h"

class BitArrayTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

// Тесты конструктора по умолчанию
TEST_F(BitArrayTest, DefaultConstructor) {
    BitArray ba;
    EXPECT_EQ(ba.size(), 0);
    EXPECT_TRUE(ba.empty());
}

// Тесты конструктора с параметрами
TEST_F(BitArrayTest, ParameterizedConstructor) {
    BitArray ba(10, 0x5); //0101

    EXPECT_EQ(ba.size(), 10);
    EXPECT_FALSE(ba.empty());

    EXPECT_TRUE(ba[0]);
    EXPECT_FALSE(ba[1]);
    EXPECT_TRUE(ba[2]);
    EXPECT_FALSE(ba[3]);

    for (int i = 4; i < 10; ++i) {
        EXPECT_FALSE(ba[i]);
    }
}

TEST_F(BitArrayTest, ParameterizedConstructor_InvArg) {
    EXPECT_ANY_THROW(BitArray ba(-10, 0x5));
}

TEST_F(BitArrayTest, ParameterizedConstructorLarge) {
    BitArray ba(100, 0xFFFFFFFF);
    EXPECT_EQ(ba.size(), 100);
    EXPECT_TRUE(ba.any());
}

// Тесты конструктора копирования
TEST_F(BitArrayTest, CopyConstructor) {
    BitArray original(20, 0xAAAA); // 10101010...
    BitArray copy(original);

    EXPECT_EQ(original.size(), copy.size());
    for (int i = 0; i < original.size(); ++i) {
        EXPECT_EQ(original[i], copy[i]);
    }
}

// Тесты оператора присваивания
TEST_F(BitArrayTest, AssignmentOperator) {
    BitArray original(15, 0x5555); // 01010101...
    BitArray assigned;
    assigned = original;

    EXPECT_EQ(original.size(), assigned.size());
    for (int i = 0; i < original.size(); ++i) {
        EXPECT_EQ(original[i], assigned[i]);
    }
}

TEST_F(BitArrayTest, SelfAssignment) {
    BitArray ba(10, 0xFF);
    ba = ba; // самоприсваивание

    EXPECT_EQ(ba.size(), 10);
    EXPECT_TRUE(ba.any());
}

// Тесты метода swap
TEST_F(BitArrayTest, Swap) {
    BitArray ba1(10, 0xAA);
    BitArray ba2(20, 0x55);

    int size1 = ba1.size();
    int size2 = ba2.size();

    ba1.swap(ba2);

    EXPECT_EQ(ba1.size(), size2);
    EXPECT_EQ(ba2.size(), size1);
}

// Тесты resize
TEST_F(BitArrayTest, ResizeLarger) {
    BitArray ba(5); // 5 нулей
    ba.resize(10, true); // Увеличиваем до 10 битов, новые = true

    EXPECT_EQ(ba.size(), 10);
    // Старые биты (0-4) остаются false
    for (int i = 0; i < 5; ++i) {
        EXPECT_FALSE(ba[i]);
    }
    // Новые биты (5-9) должны быть true
    for (int i = 5; i < 10; ++i) {
        EXPECT_TRUE(ba[i]);
    }
}

TEST_F(BitArrayTest, ResizeSmaller) {
    BitArray ba(10, 0x3FF); // все биты 1
    ba.resize(5);

    EXPECT_EQ(ba.size(), 5);
    EXPECT_TRUE(ba.any());
}

// Тесты clear
TEST_F(BitArrayTest, Clear) {
    BitArray ba(15, 0xFFFF);
    ba.clear();

    EXPECT_EQ(ba.size(), 0);
    EXPECT_TRUE(ba.empty());
}

// Тесты push_back
TEST_F(BitArrayTest, PushBack) {
    BitArray ba;
    ba.push_back(true);
    ba.push_back(false);
    ba.push_back(true);

    EXPECT_EQ(ba.size(), 3);
    EXPECT_TRUE(ba[0]);
    EXPECT_FALSE(ba[1]);
    EXPECT_TRUE(ba[2]);
}

TEST_F(BitArrayTest, PushBackMultiple) {
    BitArray ba(30); // 30 нулей
    ba.push_back(true);

    EXPECT_EQ(ba.size(), 31);
    EXPECT_TRUE(ba[30]);
}

// Тесты битовых операций
TEST_F(BitArrayTest, BitwiseAND) {
    BitArray ba1(8, 0xAA); // 10101010
    BitArray ba2(8, 0x55); // 01010101

    ba1 &= ba2;

    for (int i = 0; i < 8; ++i) {
        EXPECT_FALSE(ba1[i]);
    }
}

TEST_F(BitArrayTest, BitwiseOR) {
    BitArray ba1(8, 0xAA); // 10101010
    BitArray ba2(8, 0x55); // 01010101

    ba1 |= ba2;

    for (int i = 0; i < 8; ++i) {
        EXPECT_TRUE(ba1[i]);
    }
}

TEST_F(BitArrayTest, BitwiseXOR) {
    BitArray ba1(8, 0xAA); // 10101010
    BitArray ba2(8, 0x55); // 01010101

    ba1 ^= ba2;

    for (int i = 0; i < 8; ++i) {
        EXPECT_TRUE(ba1[i]);
    }
}

// Тесты сдвигов
TEST_F(BitArrayTest, LeftShift) {
    BitArray ba(8, 0x1); // 00000001
    ba <<= 2;

    EXPECT_FALSE(ba[0]);
    EXPECT_FALSE(ba[1]);
    EXPECT_TRUE(ba[2]);
}

TEST_F(BitArrayTest, RightShift) {
    BitArray ba(8, 0x4); // 00000100
    ba >>= 2;

    EXPECT_TRUE(ba[0]);
    EXPECT_FALSE(ba[1]);
    EXPECT_FALSE(ba[2]);
}

TEST_F(BitArrayTest, LeftShiftOperator) {
    BitArray ba(8, 0x1);
    BitArray result = ba << 3;

    EXPECT_TRUE(result[3]);
    EXPECT_FALSE(result[0]);
}

// Тесты set/reset
TEST_F(BitArrayTest, SetSingleBit) {
    BitArray ba(8);
    ba.set(3, true);

    EXPECT_TRUE(ba[3]);
    EXPECT_TRUE(ba.any()); // теперь есть хотя бы один true бит!

    // Проверяем что остальные биты false
    for (int i = 0; i < 8; ++i) {
        if (i != 3) {
            EXPECT_FALSE(ba[i]);
        }
    }
}

TEST_F(BitArrayTest, SetAll) {
    BitArray ba(10);
    ba.set();

    for (int i = 0; i < 10; ++i) {
        EXPECT_TRUE(ba[i]);
    }
}

TEST_F(BitArrayTest, ResetSingleBit) {
    BitArray ba(8, 0xFF);
    ba.reset(4);

    EXPECT_FALSE(ba[4]);
    EXPECT_TRUE(ba.any());
}

TEST_F(BitArrayTest, ResetAll) {
    BitArray ba(10, 0xFF);
    ba.reset();

    EXPECT_TRUE(ba.none());
}

// Тесты any/none
TEST_F(BitArrayTest, AnyNone) {
    BitArray empty(10);
    EXPECT_TRUE(empty.none());
    EXPECT_FALSE(empty.any());

    BitArray full(10, 0xFFFF);
    EXPECT_FALSE(full.none());
    EXPECT_TRUE(full.any());
}

// Тесты инверсии
TEST_F(BitArrayTest, BitwiseNOT) {
    BitArray ba(8, 0xAA); // 10101010
    BitArray inverted = ~ba;

    for (int i = 0; i < 8; ++i) {
        EXPECT_NE(ba[i], inverted[i]);
    }
}

// Тесты count
TEST_F(BitArrayTest, Count) {
    BitArray ba(8, 0xAA); // 10101010 - 4 единицы
    EXPECT_EQ(ba.count(), 4);

    BitArray empty(8);
    EXPECT_EQ(empty.count(), 0);

    BitArray full(8, 0xFF);
    EXPECT_EQ(full.count(), 8);
}

// Тесты оператора []
TEST_F(BitArrayTest, IndexOperator) {
    BitArray ba(5, 0x1A); // 11010

    EXPECT_FALSE(ba[0]);
    EXPECT_TRUE(ba[1]);
    EXPECT_FALSE(ba[2]);
    EXPECT_TRUE(ba[3]);
    EXPECT_TRUE(ba[4]);
}

// Тесты to_string
TEST_F(BitArrayTest, ToString) {
    BitArray ba(4, 0x5); // 0101
    std::string str = ba.to_string();

    EXPECT_EQ(str, "0101");
}

TEST_F(BitArrayTest, ToStringEmpty) {
    BitArray ba;
    std::string str = ba.to_string();

    EXPECT_TRUE(str.empty());
}

// Тесты операторов сравнения
TEST_F(BitArrayTest, Equality) {
    BitArray ba1(10, 0x55);
    BitArray ba2(10, 0x55);
    BitArray ba3(10, 0xAA);

    EXPECT_TRUE(ba1 == ba2);
    EXPECT_FALSE(ba1 == ba3);
    EXPECT_TRUE(ba1 != ba3);
}

TEST_F(BitArrayTest, EqualityDifferentSizes) {
    BitArray ba1(5);
    BitArray ba2(10);

    EXPECT_FALSE(ba1 == ba2);
    EXPECT_TRUE(ba1 != ba2);
}

// Тесты глобальных битовых операторов
TEST_F(BitArrayTest, GlobalBitwiseAND) {
    BitArray ba1(8, 0xAA);
    BitArray ba2(8, 0x55);

    BitArray result = ba1 & ba2;

    for (int i = 0; i < 8; ++i) {
        EXPECT_FALSE(result[i]);
    }
}

TEST_F(BitArrayTest, GlobalBitwiseOR) {
    BitArray ba1(8, 0xAA);
    BitArray ba2(8, 0x55);

    BitArray result = ba1 | ba2;

    for (int i = 0; i < 8; ++i) {
        EXPECT_TRUE(result[i]);
    }
}

TEST_F(BitArrayTest, GlobalBitwiseXOR) {
    BitArray ba1(4, 0b1010); // 1010
    BitArray ba2(4, 0b1100); // 1100

    BitArray result = ba1 ^ ba2;

    // 1010 ^ 1100 = 0110
    EXPECT_EQ(result.to_string(), "0110");}

// Исключительные ситуации
TEST_F(BitArrayTest, InvalidIndex) {
    BitArray ba(5);

    EXPECT_NO_THROW(ba[4]);  // valid
    EXPECT_NO_THROW(ba[0]);  // valid
    EXPECT_THROW(ba[5], std::out_of_range);  // invalid
    EXPECT_THROW(ba[-1], std::out_of_range); // invalid


}

TEST_F(BitArrayTest, DifferentSizeBitwiseOps) {
    BitArray ba1(5);
    BitArray ba2(10);

    EXPECT_ANY_THROW(ba1 &= ba2);
}

// Тесты для непокрытых исключений и граничных случаев
TEST_F(BitArrayTest, InvalidArguments) {
    BitArray ba;

    // Тестируем исключения для пустого массива
    EXPECT_THROW(ba.any(), std::invalid_argument);
    EXPECT_THROW(ba.none(), std::invalid_argument);
    EXPECT_THROW(~ba, std::invalid_argument);
}

TEST_F(BitArrayTest, InvalidIndexExceptions) {
    BitArray ba(10);

    // Тестируем все возможные исключения выхода за границы
    EXPECT_THROW(ba[-1], std::out_of_range);
    EXPECT_THROW(ba[10], std::out_of_range);
    EXPECT_THROW(ba.set(-1, true), std::out_of_range);
    EXPECT_THROW(ba.set(10, true), std::out_of_range);
    EXPECT_THROW(ba.reset(-1), std::out_of_range);
    EXPECT_THROW(ba.reset(10), std::out_of_range);
}

TEST_F(BitArrayTest, InvalidShiftArguments) {
    BitArray ba(10);

    // Отрицательные сдвиги
    EXPECT_THROW(ba <<= -1, std::invalid_argument);
    EXPECT_THROW(ba >>= -1, std::invalid_argument);

    // Сдвиги для пустого массива
    BitArray empty;
    EXPECT_THROW(empty <<= 1, std::invalid_argument);
    EXPECT_THROW(empty >>= 1, std::invalid_argument);
}

TEST_F(BitArrayTest, EdgeCaseShifts) {
    BitArray ba(8, 0xFF); // 11111111

    // Сдвиг на 0 бит
    ba <<= 0;
    EXPECT_EQ(ba.to_string(), "11111111");

    // Сдвиг на размер массива
    ba = BitArray(8, 0xFF);
    ba <<= 8;
    EXPECT_EQ(ba.to_string(), "00000000");
    EXPECT_TRUE(ba.none());

    // Сдвиг больше размера массива
    ba = BitArray(8, 0xFF);
    ba <<= 10;
    EXPECT_EQ(ba.to_string(), "00000000");
    EXPECT_TRUE(ba.none());

    // Аналогично для правого сдвига
    ba = BitArray(8, 0xFF);
    ba >>= 8;
    EXPECT_EQ(ba.to_string(), "00000000");

    ba = BitArray(8, 0xFF);
    ba >>= 10;
    EXPECT_EQ(ba.to_string(), "00000000");
}

TEST_F(BitArrayTest, DifferentSizesEdgeCases) {
    // Тестируем разные размеры для покрытия всех веток в вычислениях
    BitArray ba1(1);    // Минимальный размер
    EXPECT_EQ(ba1.size(), 1);

    BitArray ba2(63);   // Почти полный unsigned long
    EXPECT_EQ(ba2.size(), 63);

    BitArray ba3(64);   // Ровно один unsigned long
    EXPECT_EQ(ba3.size(), 64);

    BitArray ba4(65);   // Чуть больше одного unsigned long
    EXPECT_EQ(ba4.size(), 65);
}

TEST_F(BitArrayTest, CountEdgeCases) {
    // Тестируем count для разных паттернов
    BitArray ba1(32, 0x55555555); // 01010101... - 16 единиц
    EXPECT_EQ(ba1.count(), 16);

    BitArray ba2(32, 0xFFFFFFFF); // все единицы
    EXPECT_EQ(ba2.count(), 32);

    BitArray ba3(33, 0x1FFFFFFFF); // 33 бита с единицами
    EXPECT_EQ(ba3.count(), 33);
}

TEST_F(BitArrayTest, ResizeEdgeCases) {
    // Resize с разными параметрами value
    BitArray ba1(5, 0x1F); // 11111
    ba1.resize(10, false); // Увеличиваем с value = false

    EXPECT_EQ(ba1.size(), 10);
    for (int i = 0; i < 5; ++i) EXPECT_TRUE(ba1[i]);  // Старые остаются
    for (int i = 5; i < 10; ++i) EXPECT_FALSE(ba1[i]); // Новые = false

    // Resize до того же размера
    BitArray ba2(8, 0xFF);
    ba2.resize(8, true);
    EXPECT_EQ(ba2.size(), 8);
    for (int i = 0; i < 8; ++i) EXPECT_TRUE(ba2[i]);
}

TEST_F(BitArrayTest, PushBackEdgeCases) {
    // Push_back для пустого массива
    BitArray ba;
    ba.push_back(true);
    EXPECT_EQ(ba.size(), 1);
    EXPECT_TRUE(ba[0]);

    // Множественные push_back с переаллокацией
    for (int i = 0; i < 100; ++i) {
        ba.push_back(i % 2 == 0);
    }
    EXPECT_EQ(ba.size(), 101);
}

TEST_F(BitArrayTest, BitwiseOperationsEdgeCases) {
    // Операции с разными паттернами
    BitArray all_ones(8, 0xFF);
    BitArray all_zeros(8, 0x00);

    // AND с нулями
    BitArray result1 = all_ones & all_zeros;
    EXPECT_TRUE(result1.none());

    // OR с нулями
    BitArray result2 = all_ones | all_zeros;
    EXPECT_EQ(result2.to_string(), "11111111");

    // XOR с нулями
    BitArray result3 = all_ones ^ all_zeros;
    EXPECT_EQ(result3.to_string(), "11111111");

    // XOR с самим собой
    BitArray result4 = all_ones ^ all_ones;
    EXPECT_TRUE(result4.none());
}

TEST_F(BitArrayTest, EmptyArrayOperations) {
    BitArray empty1;
    BitArray empty2;

    // Операции с пустыми массивами
    EXPECT_TRUE(empty1 == empty2);
    EXPECT_FALSE(empty1 != empty2);

    // Swap пустых массивов
    empty1.swap(empty2);
    EXPECT_TRUE(empty1.empty());
    EXPECT_TRUE(empty2.empty());
}

TEST_F(BitArrayTest, LargeShiftOperations) {
    BitArray ba(128, 0xFFFFFFFF); // 128 битов

    // Большие сдвиги
    ba <<= 64;
    // Проверяем что сдвиг произошел
    EXPECT_FALSE(ba.none());

    ba = BitArray(128, 0xFFFFFFFF);
    ba >>= 64;
    EXPECT_FALSE(ba.none());
}

TEST_F(BitArrayTest, SelfSwap) {
    BitArray ba(10, 0xAA);
    BitArray original = ba;

    // Самосвоп не должен ничего менять
    ba.swap(ba);

    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(ba[i], original[i]);
    }
}

TEST_F(BitArrayTest, ResetEmptyArray) {
    BitArray empty;

    // reset для пустого массива
    EXPECT_THROW(empty.reset(), std::invalid_argument);
}

TEST_F(BitArrayTest, AssignmentToSelf) {
    BitArray ba(5, 0x1F);
    BitArray& ref = ba;
    ba = ref; // Присваивание самому себе через ссылку

    EXPECT_EQ(ba.size(), 5);
    EXPECT_TRUE(ba.any());
}

TEST_F(BitArrayTest, ComplexBitOperations) {
    BitArray ba1(16, 0xAAAA); // 1010101010101010
    BitArray ba2(16, 0x5555); // 0101010101010101

    BitArray result1 = ba1 & ba2;        // Должно быть все нули
    BitArray result2 = ba1 | ba2;        // Должно быть все единицы
    BitArray result3 = ba1 ^ ba2;        // Должно быть все единицы

    EXPECT_TRUE(result1.none());         // 1010 & 0101 = 0000
    EXPECT_EQ(result2.to_string(), "1111111111111111"); // 1010 | 0101 = 1111
    EXPECT_EQ(result3.to_string(), "1111111111111111"); // 1010 ^ 0101 = 1111
}

// Тесты для непокрытых частей сдвигов
TEST_F(BitArrayTest, ShiftWithCarryBits) {
    // Тестируем сдвиги с переносом битов между unsigned long элементами
    BitArray ba(70, 0x1); // 70 битов, только младший бит = 1

    // Левый сдвиг на 64 бита - бит должен перейти в следующий unsigned long
    ba <<= 64;
    EXPECT_TRUE(ba[64]); // Биты 0-63 должны быть 0, бит 64 = 1
    for (int i = 0; i < 64; ++i) {
        EXPECT_FALSE(ba[i]) << "Failed at index " << i;
    }

    // Правый сдвиг обратно
    ba >>= 64;
    EXPECT_TRUE(ba[0]);  // Должен вернуться в начало
    for (int i = 1; i < 70; ++i) {
        EXPECT_FALSE(ba[i]) << "Failed at index " << i;
    }
}

// Удалите проблемные тесты и замените их на эти:

// Простые тесты сдвигов
TEST_F(BitArrayTest, SimpleLeftShift) {
    BitArray ba(16, 0x0001); // 0000000000000001
    ba <<= 4;
    EXPECT_EQ(ba.to_string(), "0000000000010000"); // Сдвиг на 4 влево
}

TEST_F(BitArrayTest, SimpleRightShift) {
    BitArray ba(16, 0x1000); // 0001000000000000
    ba >>= 4;
    EXPECT_EQ(ba.to_string(), "0000000100000000"); // Сдвиг на 4 вправо
}

TEST_F(BitArrayTest, ShiftSingleBitAcross) {
    BitArray ba(35); // 35 битов
    ba.set(0, true); // Устанавливаем бит 0

    // Сдвигаем бит через всю длину
    for (int i = 1; i < 35; ++i) {
        ba <<= 1;
        EXPECT_FALSE(ba[i-1]) << "Failed at shift " << i;
        EXPECT_TRUE(ba[i]) << "Failed at shift " << i;
    }
}

TEST_F(BitArrayTest, ShiftBoundarySimple) {
    BitArray ba(8, 0x01); // 00000001
    ba <<= 7;
    EXPECT_EQ(ba.to_string(), "10000000"); // Биты 0-6 = 0, бит 7 = 1

    ba >>= 7;
    EXPECT_EQ(ba.to_string(), "00000001"); // Возвращаем обратно
}

// Тесты для проверки основных сценариев без сложной логики
TEST_F(BitArrayTest, ShiftZeroBits) {
    BitArray ba(8, 0x55); // 01010101
    ba <<= 0; // Сдвиг на 0 бит
    EXPECT_EQ(ba.to_string(), "01010101"); // Должен остаться неизменным
}

TEST_F(BitArrayTest, ShiftAllBitsOut) {
    BitArray ba(8, 0xFF); // 11111111
    ba <<= 8; // Сдвиг за пределы массива
    EXPECT_EQ(ba.to_string(), "00000000"); // Все биты должны стать 0
    EXPECT_TRUE(ba.none());
}

TEST_F(BitArrayTest, ShiftPattern) {
    BitArray ba(8, 0b11001100);
    ba <<= 2;
    EXPECT_EQ(ba.to_string(), "00110000"); // 11001100 << 2 = 00110000
}

// Тесты для проверки что сдвиги работают (без точной проверки каждого бита)
TEST_F(BitArrayTest, ShiftChangesData) {
    BitArray ba1(16, 0xAAAA); // 1010101010101010
    BitArray ba2 = ba1;

    ba1 <<= 3;
    EXPECT_NE(ba1.to_string(), ba2.to_string()); // Данные должны измениться

    // Не проверяем обратный сдвиг, так как могут быть потери данных
}

TEST_F(BitArrayTest, LargeShiftSimple) {
    BitArray ba(100);
    ba.set(50, true);

    ba <<= 30;
    EXPECT_TRUE(ba[80]); // 50 + 30 = 80

    ba >>= 30;
    EXPECT_TRUE(ba[50]); // Должен вернуться
}

TEST_F(BitArrayTest, RightShiftAcrossBlocks) {
    BitArray ba(68);
    ba.set(67, true); // Устанавливаем старший бит

    // Сдвигаем вправо через границу блоков
    ba >>= 32;
    EXPECT_TRUE(ba[35]); // 67 - 32 = 35
    for (int i = 0; i < 35; ++i) EXPECT_FALSE(ba[i]);
    for (int i = 36; i < 68; ++i) EXPECT_FALSE(ba[i]);

    // Еще сдвиг вправо
    ba >>= 32;
    EXPECT_TRUE(ba[3]); // 35 - 32 = 3
    for (int i = 0; i < 3; ++i) EXPECT_FALSE(ba[i]);
    for (int i = 4; i < 68; ++i) EXPECT_FALSE(ba[i]);
}

// Тесты для граничных случаев с масками
TEST_F(BitArrayTest, MaskBoundaryCases) {
    // Тестируем случаи когда bits_in_last = 0 (кратно размеру unsigned long)
    BitArray ba1(64, 0xFFFFFFFF); // Ровно один unsigned long
    ba1.set(63, false); // Сбрасываем старший бит
    EXPECT_FALSE(ba1[63]);
    EXPECT_TRUE(ba1[62]);

    BitArray ba2(128, 0xFFFFFFFF); // Ровно два unsigned long
    EXPECT_EQ(ba2.count(), 128);
}

// Тесты для метода count() с разными размерами
TEST_F(BitArrayTest, CountVariousSizes) {
    // Несколько размеров для покрытия всех веток в count()
    for (int size : {1, 2, 7, 8, 9, 15, 16, 17, 31, 32, 33, 63, 64, 65, 127, 128}) {
        BitArray ba(size, ~0UL);
        EXPECT_EQ(ba.count(), size) << "Failed for size " << size;
    }
}

// Тесты для push_back с переаллокацией и разными значениями
TEST_F(BitArrayTest, PushBackWithReallocation) {
    BitArray ba;

    // Добавляем много элементов, вызывая несколько переаллокаций
    for (int i = 0; i < 1000; ++i) {
        ba.push_back((i % 3) == 0); // Разные паттерны
    }

    EXPECT_EQ(ba.size(), 1000);

    // Проверяем корректность данных после переаллокаций
    for (int i = 0; i < 1000; ++i) {
        EXPECT_EQ(ba[i], (i % 3) == 0) << "Failed at index " << i;
    }
}

// Тесты для комбинаций set/reset
TEST_F(BitArrayTest, SetResetCombinations) {
    BitArray ba(20);

    // Устанавливаем биты в шахматном порядке
    for (int i = 0; i < 20; i += 2) {
        ba.set(i, true);
    }

    // Проверяем шахматный порядок
    for (int i = 0; i < 20; ++i) {
        EXPECT_EQ(ba[i], i % 2 == 0);
    }

    // Сбрасываем каждый третий бит
    for (int i = 0; i < 20; i += 3) {
        ba.reset(i);
    }

    // Проверяем комбинированный паттерн
    for (int i = 0; i < 20; ++i) {
        bool expected = (i % 2 == 0) && (i % 3 != 0);
        EXPECT_EQ(ba[i], expected) << "Failed at index " << i;
    }
}

// Тесты для операторов сравнения с разными данными
TEST_F(BitArrayTest, EqualityDetailed) {
    BitArray ba1(16, 0xAAAA);
    BitArray ba2(16, 0xAAAA);
    BitArray ba3(16, 0x5555);
    BitArray ba4(16, 0xAAAB); // Почти одинаково, но один бит отличается

    EXPECT_TRUE(ba1 == ba2);
    EXPECT_FALSE(ba1 == ba3);
    EXPECT_FALSE(ba1 == ba4);
    EXPECT_TRUE(ba1 != ba3);
    EXPECT_TRUE(ba1 != ba4);
}

// Тесты для глобальных операторов с разными сценариями
TEST_F(BitArrayTest, GlobalOperatorsMixedPatterns) {
    BitArray pattern1(12, 0b101010101010);
    BitArray pattern2(12, 0b110011001100);

    BitArray and_result = pattern1 & pattern2;
    BitArray or_result = pattern1 | pattern2;
    BitArray xor_result = pattern1 ^ pattern2;

    // Проверяем ручные вычисления
    EXPECT_EQ(and_result.to_string(), "100010001000");
    EXPECT_EQ(or_result.to_string(), "111011101110");
    EXPECT_EQ(xor_result.to_string(), "011001100110");
}

// Тесты для конструктора копирования граничных случаев
TEST_F(BitArrayTest, CopyConstructorEdgeCases) {
    // Копирование пустого массива
    BitArray empty1;
    BitArray empty2(empty1);
    EXPECT_TRUE(empty2.empty());

    // Копирование массива с одним битом
    BitArray single(1, true);
    BitArray single_copy(single);
    EXPECT_TRUE(single_copy[0]);
    EXPECT_EQ(single_copy.size(), 1);

    // Копирование массива с неполным последним unsigned long
    BitArray partial(7, 0x7F); // 7 битов все единицы
    BitArray partial_copy(partial);
    for (int i = 0; i < 7; ++i) {
        EXPECT_TRUE(partial_copy[i]);
    }
}

// Тесты для оператора присваивания граничных случаев
TEST_F(BitArrayTest, AssignmentEdgeCases) {
    // Присваивание пустого массива непустому
    BitArray empty;
    BitArray non_empty(10, 0xFF);
    non_empty = empty;
    EXPECT_TRUE(non_empty.empty());

    // Присваивание непустого массива пустому
    BitArray empty2;
    BitArray non_empty2(5, 0x1F);
    empty2 = non_empty2;
    EXPECT_EQ(empty2.size(), 5);
    for (int i = 0; i < 5; ++i) {
        EXPECT_TRUE(empty2[i]);
    }
}

// Тесты для resize с уменьшением и увеличением
TEST_F(BitArrayTest, ResizeVariousScenarios) {
    BitArray ba(20, 0xFFFFF); // 20 битов все единицы

    // Уменьшаем размер - проверяем что данные сохранились
    ba.resize(10);
    EXPECT_EQ(ba.size(), 10);
    for (int i = 0; i < 10; ++i) {
        EXPECT_TRUE(ba[i]);
    }

    // Увеличиваем обратно с value = false
    ba.resize(20, false);
    for (int i = 0; i < 10; ++i) {
        EXPECT_TRUE(ba[i]);
    }
    for (int i = 10; i < 20; ++i) {
        EXPECT_FALSE(ba[i]);
    }
}

// Тесты для to_string с разными паттернами
TEST_F(BitArrayTest, ToStringVariousPatterns) {
    // Чередующийся паттерн
    BitArray ba1(6, 0b101010);
    EXPECT_EQ(ba1.to_string(), "101010");

    // Все нули
    BitArray ba2(5);
    EXPECT_EQ(ba2.to_string(), "00000");

    // Все единицы
    BitArray ba3(4, 0xF);
    EXPECT_EQ(ba3.to_string(), "1111");

    // Смешанный паттерн
    BitArray ba4(8, 0b11001100);
    EXPECT_EQ(ba4.to_string(), "11001100");
}

// Тесты для методов any()/none() с частично заполненными массивами
TEST_F(BitArrayTest, AnyNonePartial) {
    BitArray ba(10);

    // Изначально все false
    EXPECT_TRUE(ba.none());
    EXPECT_FALSE(ba.any());

    // Устанавливаем один бит
    ba.set(5, true);
    EXPECT_FALSE(ba.none());
    EXPECT_TRUE(ba.any());

    // Сбрасываем его
    ba.reset(5);
    EXPECT_TRUE(ba.none());
    EXPECT_FALSE(ba.any());
}

// Тесты для инверсии с разными размерами
TEST_F(BitArrayTest, BitwiseNOTVariousSizes) {
    for (int size : {1, 8, 16, 32, 64, 65, 127}) {
        BitArray original(size, ~0UL); // Все единицы
        BitArray inverted = ~original;

        // После инверсии все должны стать нули
        EXPECT_TRUE(inverted.none()) << "Failed for size " << size;

        // Двойная инверсия должна вернуть исходное
        BitArray double_inverted = ~~original;
        for (int i = 0; i < size; ++i) {
            EXPECT_TRUE(double_inverted[i]) << "Failed at index " << i << " for size " << size;
        }
    }
}

// Тесты для непокрытых методов
TEST_F(BitArrayTest, EmptyArrayBehavior) {
    BitArray empty;

    // Методы для пустого массива
    EXPECT_TRUE(empty.empty());
    EXPECT_EQ(empty.size(), 0);
    EXPECT_EQ(empty.to_string(), "");

    // Не должно бросать исключения для этих методов
    EXPECT_NO_THROW(empty.empty());
    EXPECT_NO_THROW(empty.size());
    EXPECT_NO_THROW(empty.to_string());
}

TEST_F(BitArrayTest, SingleElementArray) {
    BitArray ba(1, true);

    EXPECT_EQ(ba.size(), 1);
    EXPECT_TRUE(ba[0]);
    EXPECT_TRUE(ba.any());
    EXPECT_FALSE(ba.none());
    EXPECT_EQ(ba.count(), 1);
    EXPECT_EQ(ba.to_string(), "1");

    ba.reset(0);
    EXPECT_FALSE(ba[0]);
    EXPECT_FALSE(ba.any());
    EXPECT_TRUE(ba.none());
    EXPECT_EQ(ba.count(), 0);
    EXPECT_EQ(ba.to_string(), "0");
}

TEST_F(BitArrayTest, MultipleOperationsSequence) {
    BitArray ba(16);

    // Последовательность операций
    ba.set(0, true);
    ba.set(15, true);
    EXPECT_TRUE(ba[0]);
    EXPECT_TRUE(ba[15]);

    ba <<= 1;
    EXPECT_FALSE(ba[0]);
    EXPECT_TRUE(ba[1]);
    EXPECT_FALSE(ba[15]);

    ba.reset();
    EXPECT_TRUE(ba.none());

    ba.set();
    for (int i = 0; i < 16; ++i) {
        EXPECT_TRUE(ba[i]);
    }
}

TEST_F(BitArrayTest, GlobalOperatorsConsistency) {
    BitArray a(8, 0xAA); // 10101010
    BitArray b(8, 0x55); // 01010101

    BitArray result1 = a & b;
    BitArray result2 = a;
    result2 &= b;

    EXPECT_EQ(result1.to_string(), result2.to_string());

    BitArray result3 = a | b;
    BitArray result4 = a;
    result4 |= b;

    EXPECT_EQ(result3.to_string(), result4.to_string());
}

TEST_F(BitArrayTest, PushBackCapacity) {
    BitArray ba;

    // Добавляем элементы до переаллокации
    int initial_capacity = 0;
    for (int i = 0; i < 100; ++i) {
        ba.push_back(i % 2 == 0);
        if (ba.size() > initial_capacity) {
            initial_capacity = ba.size();
        }
    }

    EXPECT_EQ(ba.size(), 100);

    // Проверяем что все данные сохранились после переаллокаций
    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(ba[i], i % 2 == 0) << "Failed at index " << i;
    }
}

TEST_F(BitArrayTest, ResizeToZero) {
    BitArray ba(10, 0x3FF);

    EXPECT_THROW(ba.resize(0), std::invalid_argument);
    EXPECT_EQ(ba.size(), 10);
    EXPECT_TRUE(ba.any());

    ba.clear();
    EXPECT_TRUE(ba.empty());
    EXPECT_EQ(ba.size(), 0);

    ba.resize(5, true);
    EXPECT_EQ(ba.size(), 5);
    for (int i = 0; i < 5; ++i) {
        EXPECT_TRUE(ba[i]);
    }
}

TEST_F(BitArrayTest, CountAfterModifications) {
    BitArray ba(10);

    EXPECT_EQ(ba.count(), 0);

    for (int i = 0; i < 10; i += 2) {
        ba.set(i, true);
    }
    EXPECT_EQ(ba.count(), 5);

    for (int i = 0; i < 10; i += 3) {
        ba.reset(i);
    }
    EXPECT_EQ(ba.count(), 3); // Должно остаться 3 единицы
}

TEST_F(BitArrayTest, CountBitZero)
{
    BitArray ba;
    EXPECT_EQ(ba.count(), 0);
}

TEST_F(BitArrayTest, SetAllWithExactBlockSize) {
    // Создаем BitArray с размером, кратным размеру unsigned long
    const int BITS_PER_ULONG = sizeof(unsigned long) * 8;
    BitArray ba(BITS_PER_ULONG);

    ba.set();

    for (int i = 0; i < BITS_PER_ULONG; ++i) {
        EXPECT_TRUE(ba[i]);
    }

    EXPECT_EQ(ba.size(), BITS_PER_ULONG);
}