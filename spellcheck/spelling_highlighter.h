// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//

#include <QtWidgets/QWidget> // input_fields.h

#include "base/timer.h"
#include "spellcheck/platform/platform_spellcheck.h"
#include "spellcheck/spellcheck_types.h"
#include "ui/ph.h"
#include "ui/widgets/input_fields.h"

#include <QtGui/QSyntaxHighlighter>
#include <QtWidgets/QMenu>
#include <QtWidgets/QTextEdit>

#include <rpl/event_stream.h>

namespace Ui {
struct ExtendedContextMenu;
} // namespace Ui

namespace ph {

extern phrase lng_spellchecker_add;
extern phrase lng_spellchecker_remove;
extern phrase lng_spellchecker_ignore;

} // namespace ph

namespace Spellchecker {

////// Phrases.

inline constexpr auto kPhrasesCount = 3;

inline void SetPhrases(ph::details::phrase_value_array<kPhrasesCount> data) {
	ph::details::set_values(std::move(data));
}

//////

class SpellingHighlighter final : public QSyntaxHighlighter {

public:
	SpellingHighlighter(
		not_null<Ui::InputField*> field,
		rpl::producer<bool> enabled);

	void contentsChange(int pos, int removed, int added);
	void checkCurrentText();
	bool enabled();

	auto contextMenuCreated() {
		return _contextMenuCreated.events();
	}

	// Windows system spellchecker forces us to perform spell operations
	// In another thread, so the word check and getting a list of suggestions
	// Are run asynchronously.
	// And then the context menu is filled in the main thread.
	void addSpellcheckerActions(
		not_null<QMenu*> menu,
		QTextCursor cursorForPosition,
		Fn<void()> showMenuCallback);

protected:
	void highlightBlock(const QString &text) override;
	bool eventFilter(QObject *o, QEvent *e) override;

private:
	void setEnabled(bool enabled);
	void checkText(const QString &text);

	void invokeCheckText(
		const QString &text,
		Fn<void(const MisspelledWords &ranges)> callback,
		int rangesOffset = 0);

	void checkChangedText();
	bool checkSingleWord(const MisspelledWord &range);
	MisspelledWords filterSkippableWords(MisspelledWords &ranges);

	bool hasUnspellcheckableTag(int begin, int length);
	MisspelledWord getWordUnderPosition(int position);
	QString documentText();

	QTextCharFormat _misspelledFormat;
	QTextCursor _cursor;

	MisspelledWords _cachedRanges;

	int _addedSymbols = 0;
	int _removedSymbols = 0;
	int _lastPosition = 0;
	bool _enabled = true;

	base::Timer _coldSpellcheckingTimer;

	not_null<Ui::InputField*> _field;
	not_null<QTextEdit*> _textEdit;

	rpl::lifetime _lifetime;

	using ContextMenu = Ui::InputField::ExtendedContextMenu;
	rpl::event_stream<ContextMenu> _contextMenuCreated;

};

} // namespace Spellchecker