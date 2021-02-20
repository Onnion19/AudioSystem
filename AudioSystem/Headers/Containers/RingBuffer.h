#include <array>
#include <cstdint>
#include <type_traits>

namespace container {

/**
 * Ring Buffer implementation.
 * A buffer made of N parts of the same size. The buffer allows partition shift.
 *  The best way to understand it's behavior is via example: A buffer made of 3
 * parts of 5 floats. This means the whole buffer is made of 15floats:
 *  - The first part will use from 0 element to 4th.
 *  - The second part will use from 5th element to 9th.
 *  - The third part will use from 10th to 14th element.
 *
 * When shfit method is called then:
 *   - The first part will use from 5th element to 9th.
 *  - The second part will use from 10th to 14th element.
 *  - The third part will use from 0 element to 4th.
 *
 * Here we can see how the buffer have been shifted.
 *
 * @Note: User never needs to care about where the partition is actually
 * pointing, this is automatically handled.
 * @param _Type data type of the buffer
 * @param _Partitions How many partitions will have the buffer
 * @param _Size size of the partitions.
 */
template <class _Type, std::uint8_t _Partitions, std::uint32_t _Size>
class RingBuffer {
  using BufferSize = std::uint32_t;
  using BufferIterator = std::_Array_iterator < _Type, static_cast<size_t>(_Size) * _Partitions>;
  using BufferConstIterator = std::_Array_const_iterator < _Type, static_cast<size_t>(_Size) * _Partitions>;
public:
  RingBuffer() {
    static_assert(_Partitions > 0 && _Size > 0,
                  "Can't declare a Ring Buffer without partitions or size 0");
  }
  ~RingBuffer() = default;

  /**
   *  Fills a partition with the given data. This is a very fast memory
   * movement.
   * @param partitionIndex to fill (without taking shift in account)
   * @param Buffer to copy in.
   */
  void FillPartition(std::uint8_t partitionIndex,
                     std::array<_Type, _Size> buffer) {
    std::copy(buffer.begin(), buffer.end(),
              GetPartitionPointer(partitionIndex));
  }

  /**
   * Shits the Ring Buffer
   */

  void Shift() { mShifts = (mShifts + 1) % _Partitions; }

  /**
   * Get constant pointer to the partition
   * @param index to retrieve data (without shifting)
   */

  BufferConstIterator GetPartitionDataConst(std::uint8_t index) {
    return GetPartitionPointer(index);
  }

  /**
   * Get pointer to the partition
   * @param index to retrieve data (without shifting)
   */
  BufferIterator GetPartitionData(std::uint8_t index) const {
    return GetPartitionPointer(index);
  }

  /**
   * Get the parittion data in array structure
   * @param index to retrieve the data
   * @return array containing the data
   */
  [[nodiscard]] std::array<_Type, _Size>
  GetPartitionDataAsArray(std::uint8_t index) {
    std::array<_Type, _Size> partition;
    auto begin = GetPartitionPointer(index);
    std::copy(begin, begin + _Size, partition.begin());
    return partition;
  }

  /**
   *  Gets the total buffer size
   * @return buffer size
   */
  [[nodiscard]] BufferSize GetBufferSize() const { return mBuffer.size(); }

private:
  [[nodiscard]] BufferIterator GetPartitionPointer(std::uint8_t index) {
    return (isIndexInRange(index))
               ? mBuffer.begin() + (ShiftIndex(index) * _Size)
               : mBuffer.end();
  }

  bool isIndexInRange(std::uint8_t index) const { return index < _Partitions; }

  [[nodiscard]] std::uint8_t ShiftIndex(std::uint8_t index) const {
    return (index + mShifts) % _Partitions;
  }

private:
  std::array<_Type, static_cast<size_t>(_Size *_Partitions)> mBuffer;
  std::uint8_t mShifts = 0;
};

} // namespace container
