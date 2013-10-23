/* (c) Copyright 2000 International Game Technology */
/*
   $Id: SlotWinningAnimationsList.hpp,v 1.39, 2010-09-14 16:38:04Z, Erickson, Jason$
   $Date: 9/15/2010 12:38:04 AM$
   $Revision: 40$

   Purpose:
      Defines the list used to sort the wins.  Its entries consist
      of the reel and symbol positon to play the animation.  And the animation is properly configured based
      on the animation and sound selection algorithms given in the WinAnimationsList.

   Modifications:
      Author          Date         Explanation
      --------------- -----------  ------------
      Robert Breckner 22 Aug 2000  Created
*/


#ifndef __SLOTWINNINGANIMATIONSLIST_HPP__
#define __SLOTWINNINGANIMATIONSLIST_HPP__

#include <list>
#include <vector>
#ifndef __AMOUNT_HPP__
#include "Amount.hpp"
#endif
#ifndef __EVALUATORDEFS_HPP__
#include "EvaluatorDefs.hpp"
#endif
#ifndef __IGTDEFS_HPP__
#include "IGTdefs.hpp"
#endif
#ifndef __ANIMATIONEVENTPROVIDERINTERFACE_HPP__
#include "AnimationEventProviderInterface.hpp"
#endif

class SlotWinningAnimationEntry
{
   public:
      // Constructors
      SlotWinningAnimationEntry () :
         animation_priority (0),
         reel_number (0),
         offset_from_home_position (0),
         winning_symbol (false),
         symbol_name ("")
      {
      }
      SlotWinningAnimationEntry (uint32 animation_priority, int16 reel_number, int16 offset_from_home_position,
         AnimationEventProviderInterface *animation, bool winning_symbol, String symbol_name) :
         animation (animation),
         animation_priority (animation_priority),
         reel_number (reel_number),
         offset_from_home_position (offset_from_home_position),
         winning_symbol (winning_symbol),
         symbol_name (symbol_name) {}
      // Destructor.
      ~SlotWinningAnimationEntry ()
      {
         animation.release ();
      }

      Mojo<AnimationEventProviderInterface> animation;
      uint32 animation_priority;
      int16 reel_number;
      int16 offset_from_home_position;
      bool winning_symbol;
      String symbol_name;

      bool operator==(const SlotWinningAnimationEntry &rhs) const
      {
         return (reel_number == rhs.reel_number &&
            offset_from_home_position == rhs.offset_from_home_position &&
            symbol_name == rhs.symbol_name &&
            animation == rhs.animation &&
            animation_priority == rhs.animation_priority &&
            winning_symbol == rhs.winning_symbol);
      }
};

typedef vector<SlotWinningAnimationEntry> SlotWinningAnimationEntryList;
typedef SlotWinningAnimationEntryList::iterator SlotWinningAnimationEntryListIterator;


class SlotWinEntry : public SlotWinningAnimationEntryList
{
   public:
      enum WinType
      {
         INVALID,
         PAYLINE_WIN,
         SCATTER_WIN,
         MULTI_WAY_WIN,
         PATTERN_WIN,
         AUXILIARY_WIN,
         BINGO_WIN,
         BINGO_INTERIM_WIN,
         RMLP_WIN
      };

      typedef uint16 WinClassification;
      static const WinClassification WIN_CLASSIFICATION_INVALID = 0x0000;
      static const WinClassification BASIC_WIN = 0x0001;
      static const WinClassification BONUS_WIN = 0x0002;
      static const WinClassification JACKPOT_WIN = 0x0004;
      static const WinClassification UTILITY_INFO = 0x0008;  // For slot evaulation in utility mode.

      // Constructors
      SlotWinEntry () :
         animation_priority (0),
         award_index (0),
         sub_award_index (0),
         display_id (0),
         win_category (0),
         win_type (INVALID),
         win_cycle_index (0),
         win_total (ZERO_AMOUNT),
         base_win (ZERO_AMOUNT),
         multiplier (-1),
         win_classification (WIN_CLASSIFICATION_INVALID)
      {
      }

      SlotWinEntry (WinType _win_type,
                    uint16 _award_index,
                    uint32 _display_id,
                    uint32 _animation_priority,
                    uint32 _win_category,
                    uint16 _sub_award_index = 0,
                    const char *_win_string = 0,
                    uint32 _win_cycle_index = 0,
                    Amount _win_total = ZERO_AMOUNT,
                    Amount _base_win = ZERO_AMOUNT,
                    int32  _multiplier = -1,
                    uint16 _win_classification = WIN_CLASSIFICATION_INVALID) :
         animation_priority (_animation_priority),
         award_index (_award_index),
         sub_award_index (_sub_award_index),
         display_id (_display_id),
         win_category (_win_category),
         win_string (_win_string),
         win_type (_win_type),
         win_cycle_index (_win_cycle_index),
         win_total (_win_total),
         base_win (_base_win),
         multiplier (_multiplier),
         win_classification (_win_classification)
      {
      }

      // Destructor.
      ~SlotWinEntry ()
      {
         clear ();
      }

      // Comparison operator
      bool operator== (const SlotWinEntry &other) const
      {
         if (animation_priority == other.animation_priority &&
            award_index == other.award_index &&
            sub_award_index == other.sub_award_index &&
            display_id == other.display_id &&
            win_category == other.win_category &&
            win_string == other.win_string &&
            win_type == other.win_type &&
            win_cycle_index == other.win_cycle_index &&
            win_total == other.win_total &&
            base_win == other.base_win &&
            multiplier == other.multiplier &&
            win_classification == other.win_classification)
         {
            if (size() == other.size())
            {
               for (uint16 i = 0; i < size(); ++i)
               {
                  if (!(at(i) == other[i]))
                     return false;
               }
               return true;
            }
         }
         return false;
      }

      // Animation priority for 2006 standards win animations.
      uint32 animation_priority;
      // The line number, pattern number or scatter index (depending on win_type) in the slot_award represented by this win.
      uint16 award_index;
      // This value is only used by pattern wins as, unlike paylines and scatters,
      // patterns can have more than one win per pattern
      uint16 sub_award_index;
      // This is the payline or scatters configuration to use for display.
      uint32 display_id;
      WinCategoryId win_category;
      // The win string to display.
      String win_string;
      WinType  win_type;
      // Indicates the order of win-cycling - 0 means this win entry is shown first.
      uint32 win_cycle_index;
      // The total line pay.
      Amount win_total;
      // The base line pay.
      Amount base_win;
      // The line multiplier.
      int32 multiplier;
      // Indicates BASIC_WIN, BONUS_WIN, JACKPOT_WIN
      uint16 win_classification;

};

typedef list<SlotWinEntry> SlotWinningEntryList;
typedef list<SlotWinEntry>::iterator SlotWinningEntryListIterator;
typedef SlotWinningAnimationEntryListIterator SlotWinEntryIterator;

#endif

