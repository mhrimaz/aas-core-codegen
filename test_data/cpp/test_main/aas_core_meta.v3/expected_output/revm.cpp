// This code has been automatically generated by aas-core-codegen.
// Do NOT edit or append.

#include "aas_core/aas_3_0/common.hpp"
#include "aas_core/aas_3_0/revm.hpp"

#pragma warning(push, 0)
#include <algorithm>
#include <iomanip>
#include <sstream>
#pragma warning(pop)

namespace aas_core {
namespace aas_3_0 {
namespace revm {

/**
 * Represent the character such that it can be printed in the console.
 *
 * To that end, we escape the character if it is out of the printable ASCII set.
 *
 * This function is mostly meant for debugging purposes.
 */
std::string RepresentWCharacter(wchar_t character) {
  switch (character) {
    case L'\\':return "\\\\";
    case L'"':return "\\\"";
    case L'\'':return "\\'";
    case L'\t':return "\\t";
    case L'\n':return "\\n";
    case L'\r':return "\\r";
    default: break;
  }

  if (26 <= character && character <= 126) {
    return std::string(1, static_cast<char>(character));
  }

  static const char* digits = "0123456789ABCDEF";
  size_t digit_count = sizeof(wchar_t) * 2;

  std::string result;
  result.resize(digit_count + 2);
  result[0] = L'\\';
  result[1] = L'u';

  for (size_t i = 0, j = (digit_count - 1) * 4; i < digit_count; ++i, j -= 4) {
    const size_t digit_i = (character >> j) & 0x0f;
    result[i + 2] = digits[digit_i];
  }

  return result;
}

/**
 * Represent the wide string for debugging purposes where it is printed to the console.
 *
 * To that end, we escape the individual characters if they are out of printable ASCII
 * set.
 */
std::string RepresentWString(const std::wstring& text) {
  std::vector<std::string> parts;
  parts.reserve(text.size());
  for (const wchar_t character : text) {
    parts.emplace_back(RepresentWCharacter(character));
  }

  size_t size = 0;
  for (const std::string& part : parts) {
    size += part.size();
  }

  std::string result;
  result.reserve(size);
  for (const std::string& part : parts) {
    result.append(part);
  }
  return result;
}

InstructionChar::InstructionChar(
  wchar_t a_character
) :
  character(a_character) {
  // Intentionally empty.
}

InstructionKind InstructionChar::kind() const {
  return InstructionKind::Char;
}

std::string to_string(const InstructionChar& instruction) {
  return common::Concat(
    "char '",
    RepresentWCharacter(instruction.character),
    "'"
  );
}

Range::Range(
  wchar_t a_first,
  wchar_t a_last
) :
  first(a_first),
  last(a_last) {
  // NOTE (mristin):
  // We are aware that exceptions in constructors should be avoided to prevent
  // bug related to uninitialized object state. However, in this case, we do not
  // see any risk for such a mistake.
  if (a_first > a_last) {
    throw std::invalid_argument(
      common::Concat(
        "The first character in a character range, ",
        RepresentWCharacter(a_first),
        ", is larger than the last character in the range, ",
        RepresentWCharacter(a_last)
      )
    );
  }
}

std::string to_string(const Range& range) {
  if (range.first == range.last) {
    return RepresentWCharacter(range.first);
  }

  return common::Concat(
    RepresentWCharacter(range.first),
    "-",
    RepresentWCharacter(range.last)
  );
}

InstructionSet::InstructionSet(
  std::vector<Range> a_ranges
) :
  ranges(std::move(a_ranges)) {
  // NOTE (mristin):
  // We are aware that exceptions in constructors should be avoided to prevent
  // bug related to uninitialized object state. However, in this case, we do not
  // see any risk for such a mistake.
  if (ranges.empty()) {
    throw std::invalid_argument(
      "Unexpected NotSet instruction with empty ranges"
    );
  }

  for (size_t i = 1; i < ranges.size(); ++i) {
    if (ranges[i - 1].last >= ranges[i].first) {
      throw std::invalid_argument(
        common::Concat(
          "The ranges for an InstructionSet are unexpectedly either "
          "not sorted or overlapping. The range at index ",
          std::to_string(i - 1),
          " is ",
          to_string(ranges[i - 1]),
          " and the range at index ",
          std::to_string(i),
          " is ",
          to_string(ranges[i])
        )
      );
    }
  }
}

InstructionKind InstructionSet::kind() const {
  return InstructionKind::Set;
}

std::string to_string(const InstructionSet& instruction) {
  std::stringstream ss;
  ss << "set '";

  for (const auto& range : instruction.ranges) {
    ss << to_string(range);
  }

  ss << "'";
  return ss.str();
}

InstructionNotSet::InstructionNotSet(
  std::vector<Range> a_ranges
) :
  ranges(std::move(a_ranges)) {
  // NOTE (mristin):
  // We are aware that exceptions in constructors should be avoided to prevent
  // bug related to uninitialized object state. However, in this case, we do not
  // see any risk for such a mistake.
  if (ranges.empty()) {
    throw std::invalid_argument(
      "Unexpected NotSet instruction with empty ranges"
    );
  }

  for (size_t i = 1; i < ranges.size(); ++i) {
    if (ranges[i - 1].last >= ranges[i].first) {
      throw std::invalid_argument(
        common::Concat(
          "The ranges for an InstructionNotSet are unexpectedly either "
          "not sorted or overlapping. The range at index ",
          std::to_string(i - 1),
          " is ",
          to_string(ranges[i - 1]),
          " and the range at index ",
          std::to_string(i),
          " is ",
          to_string(ranges[i])
        )
      );
    }
  }
}

InstructionKind InstructionNotSet::kind() const {
  return InstructionKind::NotSet;
}

std::string to_string(const InstructionNotSet& instruction) {
  std::stringstream ss;
  ss << "not-set '";

  for (const auto& range : instruction.ranges) {
    ss << to_string(range);
  }

  ss << "'";
  return ss.str();
}

InstructionKind InstructionAny::kind() const {
  return InstructionKind::Any;
}

std::string to_string(const InstructionAny&) {
  return "any";
}

InstructionKind InstructionMatch::kind() const {
  return InstructionKind::Match;
}

std::string to_string(const InstructionMatch&) {
  return "match";
}

InstructionJump::InstructionJump(
  size_t a_target
) :
  target(a_target) {
  // Intentionally empty.
}

InstructionKind InstructionJump::kind() const {
  return InstructionKind::Jump;
}

std::string to_string(const InstructionJump& instruction) {
  return common::Concat(
    "jump ",
    std::to_string(instruction.target)
  );
}

InstructionSplit::InstructionSplit(
  size_t a_first_target,
  size_t a_second_target
) :
  first_target(a_first_target),
  second_target(a_second_target) {
  // Intentionally empty.
}

InstructionKind InstructionSplit::kind() const {
  return InstructionKind::Split;
}

std::string to_string(const InstructionSplit& instruction) {
  return common::Concat(
    "split ",
      std::to_string(instruction.first_target),
      ", ",
      std::to_string(instruction.second_target)
  );
}

InstructionKind InstructionEnd::kind() const {
  return InstructionKind::End;
}

std::string to_string(const InstructionEnd&) {
  return "end";
}

std::string to_string(const Instruction& instruction) {
  switch (instruction.kind()) {
    case InstructionKind::Char:
      return to_string(
        static_cast<   // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
          const InstructionChar&
        >(instruction)
      );

    case InstructionKind::Set:
      return to_string(
        static_cast<   // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
          const InstructionSet&
        >(instruction)
      );

    case InstructionKind::NotSet:
      return to_string(
        static_cast<   // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
          const InstructionNotSet&
        >(instruction)
      );

    case InstructionKind::Any:
      return to_string(
        static_cast<   // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
          const InstructionAny&
        >(instruction)
      );

    case InstructionKind::Match:
      return to_string(
        static_cast<   // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
          const InstructionMatch&
        >(instruction)
      );

    case InstructionKind::Jump:
      return to_string(
        static_cast<   // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
          const InstructionJump&
        >(instruction)
      );

    case InstructionKind::Split:
      return to_string(
        static_cast<   // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
          const InstructionSplit&
        >(instruction)
      );

    case InstructionKind::End:
      return to_string(
        static_cast<   // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
          const InstructionEnd&
        >(instruction)
      );

    default:
      throw std::logic_error(
        common::Concat(
          "Unhandled instruction kind: ",
          std::to_string(
            static_cast<std::uint8_t>(instruction.kind())
          )
        )
      );
  }
}

std::string to_string(
  const std::vector<std::unique_ptr<Instruction> >& instructions
) {
  std::stringstream ss;
  for (size_t i = 0; i < instructions.size(); ++i) {
    ss
      << std::setw(4) << i << ": "
      << to_string(*instructions[i]) << std::endl;
  }

  return ss.str();
}

bool CharacterInRanges(
  const std::vector<Range>& ranges,
  wchar_t character
) {
  if (ranges.empty()) {
    return false;
  }

  if (ranges.size() == 1) {
    return (ranges[0].first <= character && character <= ranges[0].last);
  }

  // Binary search
  size_t begin = 0;
  size_t end = ranges.size();

  while (true) {
    if (begin == end) {
      return false;
    }

    // NOTE (mristin):
    // Most implementations of the binary search are buggy, see:
    // https://en.wikipedia.org/wiki/Binary_search_algorithm#Implementation_issues.
    //
    // We try to avert some of the bugs by explicitly handling the case where there
    // are at most 3 elements in the segment, so we stop here instead of proceeding
    // recursively.
    if (end - begin <= 3) {
      for (size_t i = begin; i < end; ++i) {
        const Range& range = ranges[i];
        if (range.first <= character && character <= range.last) {
          return true;
        }
      }
      return false;
    }

    const size_t middle = (begin + end) / 2;
    const Range& range = ranges[middle];
    if (character < range.first) {
      end = middle;
    } else if (character > range.last) {
      begin = middle;
    } else {
      return true;
    }
  }
}

/**
 * Keep track of the threads currently being executed.
 */
class ThreadList {
 public:
  explicit ThreadList(size_t program_size) {
    has_.resize(program_size, false);
    items_.reserve(program_size);
  }

  /**
   * Add a new thread for the given program counter if it is not already in the list.
   */
  void Spawn(size_t program_counter) {
    #ifdef DEBUG
    if (program_counter >= program_size_) {
      throw std::invalid_argument(
        common::Concat(
          "Unexpected spawning of a thread at the program counter ",
          std::to_string(program_counter),
          " since the program size was indicated to be ",
          std::to_string(program_size_)
        )
      );
    }
    #endif

    if (has_[program_counter]) {
      return;
    }

    has_[program_counter] = true;
    items_.push_back(program_counter);
  }

  bool Empty() const {
    return items_.empty();
  }

  /**
   * Pop the thread from the back, returning its program counter.
   *
   * The order of the threads is not guaranteed.
   */
  size_t Pop() {
    #ifdef DEBUG
    if (items_.empty()) {
      throw std::logic_error(
        "You tried to pop from an empty thread list."
        );
    }
    #endif

    const size_t program_counter = items_.back();
    items_.pop_back();
    has_[program_counter] = false;
    return program_counter;
  }

  /**
   * Clear the thread list, keeping its memory capacity.
   */
  void Clear() {
    std::fill(has_.begin(), has_.end(), false);
    items_.clear();
  }

  /**
   * Return the program counters corresponding to the spawned threads.
   */
  const std::vector<size_t>& Items() const {
    return items_;
  };

 private:
  /**
   * Keep track of the program counters corresponding to the threads so that we can
   * avoid the duplicate threads.
   */
  std::vector<bool> has_;

  /**
   * Keep track of the active threads.
   */
  std::vector<size_t> items_;
};

std::string to_string(const ThreadList& thread_list) {
  if (thread_list.Empty()) {
    return "[]";
  }

  std::vector<size_t> items(thread_list.Items());
  std::sort(items.begin(), items.end());

  std::stringstream ss;
  ss << "[";
  ss << std::to_string(items[0]);

  for (size_t i = 1; i < items.size(); ++i) {
    ss << ", " << items[i];
  }
  ss << "]";

  return ss.str();
}

/**
 * Try to match the program against the text.
 *
 * @return true if the text matches
 */
bool Match(
  const std::vector<std::unique_ptr<Instruction> >& program,
  const std::wstring& text
) {
  if (program.empty()) {
    return false;
  }

  // NOTE (mristin):
  // We validate at the beginning so that we can avoid checks in
  // the instruction loops.
  for (size_t i = 0; i < program.size(); ++i) {
    const Instruction& instruction = *program[i];

    switch (instruction.kind()) {
      case InstructionKind::Jump: {
        const auto& instruction_jump(
          static_cast<  // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
            const InstructionJump&
          >(
            instruction
          )
        );

        if (instruction_jump.target >= program.size()) {
          throw std::invalid_argument(
            common::Concat(
              "Unexpected jump beyond the end of the program. Program has ",
              std::to_string(program.size()),
              " instruction(s), but the instruction ",
              std::to_string(i),
              " wants to jump to ",
              std::to_string(instruction_jump.target)
            )
          );
        }
        break;
      }

      case InstructionKind::Split: {
        const auto& instruction_split(
          static_cast<  // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
            const InstructionSplit&
          >(
            instruction
          )
        );

        if (instruction_split.first_target >= program.size()) {
          throw std::invalid_argument(
            common::Concat(
              "Unexpected split & jump beyond the end of the program. Program has ",
              std::to_string(program.size()),
              " instruction(s), but the instruction ",
              std::to_string(i),
              " wants to split and make the first jump to ",
              std::to_string(instruction_split.first_target)
            )
          );
        }

        if (instruction_split.second_target >= program.size()) {
          throw std::invalid_argument(
            common::Concat(
              "Unexpected split & jump beyond the end of the program. Program has ",
              std::to_string(program.size()),
              " instruction(s), but the instruction ",
              std::to_string(i),
              " wants to split and make the second jump to ",
              std::to_string(instruction_split.second_target)
            )
          );
        }

        break;
      }

      default:
        continue;
    }
  }

  // NOTE (mristin):
  // See: https://swtch.com/~rsc/regexp/regexp2.html,
  // Section "Thompson's Implementation".

  std::unique_ptr<ThreadList> clist(std::make_unique<ThreadList>(program.size()));
  std::unique_ptr<ThreadList> nlist(std::make_unique<ThreadList>(program.size()));

  clist->Spawn(0);

  for (const wchar_t character : text) {
    #ifdef DEBUG
    if (!nlist->Empty()) {
      throw std::logic_error(
        "Expected the list of next-to-be-executed threads to be empty, "
        "but it was not."
      );
    }
    #endif

    while (!clist->Empty()) {
      const size_t program_counter = clist->Pop();

      #ifdef DEBUG
      if (program_counter >= program.size()) {
        throw std::logic_error(
          common::Concat(
            "Unexpected program counter beyond the program. The program size was ",
            std::to_string(program.size()),
            ", while the program counter of a thread was ",
            std::to_string(program_counter)
          )
        );
      }
      #endif

      const Instruction& instruction = *program[program_counter];

      switch (instruction.kind()) {
        case InstructionKind::Char: {
          const auto& instruction_char(
            static_cast< // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
              const InstructionChar&
            >(instruction)
          );

          if (character != instruction_char.character) {
            // The matching failed for this thread.
            break;
          }

          nlist->Spawn(program_counter + 1);
          break;
        }

        case InstructionKind::Set: {
          const auto& instruction_set(
            static_cast< // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
              const InstructionSet&
            >(instruction)
          );

          if (!CharacterInRanges(instruction_set.ranges, character)) {
            // The matching failed for this thread.
            break;
          }

          nlist->Spawn(program_counter + 1);
          break;
        }

        case InstructionKind::NotSet: {
          const auto& instruction_not_set(
            static_cast< // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
              const InstructionNotSet&
            >(instruction)
          );

          if (CharacterInRanges(instruction_not_set.ranges, character)) {
            // The matching failed for this thread.
            break;
          }

          nlist->Spawn(program_counter + 1);
          break;
        }

        case InstructionKind::Any: {
          // NOTE (mristin):
          // We simply proceed to the next instruction at the next character without
          // any checks.
          nlist->Spawn(program_counter + 1);
          break;
        }

        case InstructionKind::Match:
          return true;

        case InstructionKind::Jump: {
          const auto& instruction_jump(
            static_cast< // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
              const InstructionJump&
            >(instruction)
          );

          clist->Spawn(instruction_jump.target);
          break;
        }

        case InstructionKind::Split: {
          const auto& instruction_split(
            static_cast< // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
              const InstructionSplit&
            >(instruction)
          );

          clist->Spawn(instruction_split.first_target);
          clist->Spawn(instruction_split.second_target);
          break;
        }

        case InstructionKind::End: {
          // The matching failed for this thread as we have just consumed
          // a character.
          break;
        }

        default:
          throw std::logic_error(
            common::Concat(
              "Unhandled instruction kind: ",
              std::to_string(
                static_cast<std::uint8_t>(instruction.kind())
              )
            )
          );
      }
    }

    std::swap(clist, nlist);
    nlist->Clear();
  }

  // NOTE (mristin):
  // We need to process any pending jumps, splits and matches even tough there are
  // no more characters to consume.
  while (!clist->Empty()) {
    const size_t program_counter = clist->Pop();

    #ifdef DEBUG
    if (program_counter >= program.size()) {
      throw std::logic_error(
        common::Concat(
          "Unexpected program counter beyond the program. The program size was ",
          std::to_string(program.size()),
          ", while the program counter of a thread was ",
          std::to_string(program_counter)
        )
      );
    }
    #endif

    const Instruction& instruction = *program[program_counter];

    switch (instruction.kind()) {
      case InstructionKind::Char: { // NOLINT(bugprone-branch-clone)
        // We reached the end-of-input so there are no characters to be matched.
        // This thread needs therefore to die.
        break;
      }

      case InstructionKind::Set: {
        // We reached the end-of-input so there are no character sets to be matched.
        // This thread needs therefore to die.
        break;
      }

      case InstructionKind::NotSet: {
        // We reached the end-of-input so there are no character sets to be matched.
        // This thread needs therefore to die.
        break;
      }

      case InstructionKind::Any: {
        // We reached the end-of-input so there are no characters to be matched.
        // This thread needs therefore to die.
        break;
      }

      case InstructionKind::Match:
        return true;

      case InstructionKind::Jump: {
        const auto& instruction_jump(
          static_cast< // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
            const InstructionJump&
          >(instruction)
        );

        clist->Spawn(instruction_jump.target);
        break;
      }

      case InstructionKind::Split: {
        const auto& instruction_split(
          static_cast< // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
            const InstructionSplit&
          >(instruction)
        );

        clist->Spawn(instruction_split.first_target);
        clist->Spawn(instruction_split.second_target);
        break;
      }

      case InstructionKind::End: {
        // We reached the end-of-input so we match and move to the next instruction.
        clist->Spawn(program_counter + 1);
        break;
      }

      default:
        throw std::logic_error(
          common::Concat(
            "Unhandled instruction kind: ",
            std::to_string(
              static_cast<std::uint8_t>(instruction.kind())
            )
          )
        );
    }
  }

  return false;
}

}  // namespace revm
}  // namespace aas_3_0
}  // namespace aas_core

// This code has been automatically generated by aas-core-codegen.
// Do NOT edit or append.
