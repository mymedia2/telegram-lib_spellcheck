// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//

#include "chat_helpers/spellchecker_helper.h"
#include "platform/platform_spellcheck.h"

namespace {

struct SubtagScript {
	const char *subtag;
	QChar::Script script;
};

// https://chromium.googlesource.com/chromium/src/+/refs/heads/master/third_party/blink/renderer/platform/text/locale_to_script_mapping.cc

QChar::Script LocaleToScriptCode(const QString &locale) {
	static constexpr SubtagScript kLocaleScriptList[] = {
		{"aa", QChar::Script_Latin},
		{"ab", QChar::Script_Cyrillic},
		{"ady", QChar::Script_Cyrillic},
		{"aeb", QChar::Script_Arabic},
		{"af", QChar::Script_Latin},
		{"ak", QChar::Script_Latin},
		{"am", QChar::Script_Ethiopic},
		{"ar", QChar::Script_Arabic},
		{"arq", QChar::Script_Arabic},
		{"ary", QChar::Script_Arabic},
		{"arz", QChar::Script_Arabic},
		{"as", QChar::Script_Bengali},
		{"ast", QChar::Script_Latin},
		{"av", QChar::Script_Cyrillic},
		{"ay", QChar::Script_Latin},
		{"az", QChar::Script_Latin},
		{"azb", QChar::Script_Arabic},
		{"ba", QChar::Script_Cyrillic},
		{"bal", QChar::Script_Arabic},
		{"be", QChar::Script_Cyrillic},
		{"bej", QChar::Script_Arabic},
		{"bg", QChar::Script_Cyrillic},
		{"bi", QChar::Script_Latin},
		{"bn", QChar::Script_Bengali},
		{"bo", QChar::Script_Tibetan},
		{"bqi", QChar::Script_Arabic},
		{"brh", QChar::Script_Arabic},
		{"bs", QChar::Script_Latin},
		{"ca", QChar::Script_Latin},
		{"ce", QChar::Script_Cyrillic},
		{"ceb", QChar::Script_Latin},
		{"ch", QChar::Script_Latin},
		{"chk", QChar::Script_Latin},
		{"cja", QChar::Script_Arabic},
		{"cjm", QChar::Script_Arabic},
		{"ckb", QChar::Script_Arabic},
		{"cs", QChar::Script_Latin},
		{"cy", QChar::Script_Latin},
		{"da", QChar::Script_Latin},
		{"dcc", QChar::Script_Arabic},
		{"de", QChar::Script_Latin},
		{"doi", QChar::Script_Arabic},
		{"dv", QChar::Script_Thaana},
		{"dyo", QChar::Script_Arabic},
		{"dz", QChar::Script_Tibetan},
		{"ee", QChar::Script_Latin},
		{"efi", QChar::Script_Latin},
		{"el", QChar::Script_Greek},
		{"en", QChar::Script_Latin},
		{"es", QChar::Script_Latin},
		{"et", QChar::Script_Latin},
		{"eu", QChar::Script_Latin},
		{"fa", QChar::Script_Arabic},
		{"fi", QChar::Script_Latin},
		{"fil", QChar::Script_Latin},
		{"fj", QChar::Script_Latin},
		{"fo", QChar::Script_Latin},
		{"fr", QChar::Script_Latin},
		{"fur", QChar::Script_Latin},
		{"fy", QChar::Script_Latin},
		{"ga", QChar::Script_Latin},
		{"gaa", QChar::Script_Latin},
		{"gba", QChar::Script_Arabic},
		{"gbz", QChar::Script_Arabic},
		{"gd", QChar::Script_Latin},
		{"gil", QChar::Script_Latin},
		{"gl", QChar::Script_Latin},
		{"gjk", QChar::Script_Arabic},
		{"gju", QChar::Script_Arabic},
		{"glk", QChar::Script_Arabic},
		{"gn", QChar::Script_Latin},
		{"gsw", QChar::Script_Latin},
		{"gu", QChar::Script_Gujarati},
		{"ha", QChar::Script_Latin},
		{"haw", QChar::Script_Latin},
		{"haz", QChar::Script_Arabic},
		{"he", QChar::Script_Hebrew},
		{"hi", QChar::Script_Devanagari},
		{"hil", QChar::Script_Latin},
		{"hnd", QChar::Script_Arabic},
		{"hno", QChar::Script_Arabic},
		{"ho", QChar::Script_Latin},
		{"hr", QChar::Script_Latin},
		{"ht", QChar::Script_Latin},
		{"hu", QChar::Script_Latin},
		{"hy", QChar::Script_Armenian},
		{"id", QChar::Script_Latin},
		{"ig", QChar::Script_Latin},
		{"ii", QChar::Script_Yi},
		{"ilo", QChar::Script_Latin},
		{"inh", QChar::Script_Cyrillic},
		{"is", QChar::Script_Latin},
		{"it", QChar::Script_Latin},
		{"iu", QChar::Script_CanadianAboriginal},
		{"ja", QChar::Script_Katakana}, // or Script_Hiragana.
		{"jv", QChar::Script_Latin},
		{"ka", QChar::Script_Georgian},
		{"kaj", QChar::Script_Latin},
		{"kam", QChar::Script_Latin},
		{"kbd", QChar::Script_Cyrillic},
		{"kha", QChar::Script_Latin},
		{"khw", QChar::Script_Arabic},
		{"kk", QChar::Script_Cyrillic},
		{"kl", QChar::Script_Latin},
		{"km", QChar::Script_Khmer},
		{"kn", QChar::Script_Kannada},
		{"ko", QChar::Script_Hangul},
		{"kok", QChar::Script_Devanagari},
		{"kos", QChar::Script_Latin},
		{"kpe", QChar::Script_Latin},
		{"krc", QChar::Script_Cyrillic},
		{"ks", QChar::Script_Arabic},
		{"ku", QChar::Script_Arabic},
		{"kum", QChar::Script_Cyrillic},
		{"kvx", QChar::Script_Arabic},
		{"kxp", QChar::Script_Arabic},
		{"ky", QChar::Script_Cyrillic},
		{"la", QChar::Script_Latin},
		{"lah", QChar::Script_Arabic},
		{"lb", QChar::Script_Latin},
		{"lez", QChar::Script_Cyrillic},
		{"lki", QChar::Script_Arabic},
		{"ln", QChar::Script_Latin},
		{"lo", QChar::Script_Lao},
		{"lrc", QChar::Script_Arabic},
		{"lt", QChar::Script_Latin},
		{"luz", QChar::Script_Arabic},
		{"lv", QChar::Script_Latin},
		{"mai", QChar::Script_Devanagari},
		{"mdf", QChar::Script_Cyrillic},
		{"mfa", QChar::Script_Arabic},
		{"mg", QChar::Script_Latin},
		{"mh", QChar::Script_Latin},
		{"mi", QChar::Script_Latin},
		{"mk", QChar::Script_Cyrillic},
		{"ml", QChar::Script_Malayalam},
		{"mn", QChar::Script_Cyrillic},
		{"mr", QChar::Script_Devanagari},
		{"ms", QChar::Script_Latin},
		{"mt", QChar::Script_Latin},
		{"mvy", QChar::Script_Arabic},
		{"my", QChar::Script_Myanmar},
		{"myv", QChar::Script_Cyrillic},
		{"mzn", QChar::Script_Arabic},
		{"na", QChar::Script_Latin},
		{"nb", QChar::Script_Latin},
		{"ne", QChar::Script_Devanagari},
		{"niu", QChar::Script_Latin},
		{"nl", QChar::Script_Latin},
		{"nn", QChar::Script_Latin},
		{"nr", QChar::Script_Latin},
		{"nso", QChar::Script_Latin},
		{"ny", QChar::Script_Latin},
		{"oc", QChar::Script_Latin},
		{"om", QChar::Script_Latin},
		{"or", QChar::Script_Oriya},
		{"os", QChar::Script_Cyrillic},
		{"pa", QChar::Script_Gurmukhi},
		{"pag", QChar::Script_Latin},
		{"pap", QChar::Script_Latin},
		{"pau", QChar::Script_Latin},
		{"pl", QChar::Script_Latin},
		{"pon", QChar::Script_Latin},
		{"prd", QChar::Script_Arabic},
		{"prs", QChar::Script_Arabic},
		{"ps", QChar::Script_Arabic},
		{"pt", QChar::Script_Latin},
		{"qu", QChar::Script_Latin},
		{"rm", QChar::Script_Latin},
		{"rmt", QChar::Script_Arabic},
		{"rn", QChar::Script_Latin},
		{"ro", QChar::Script_Latin},
		{"ru", QChar::Script_Cyrillic},
		{"rw", QChar::Script_Latin},
		{"sa", QChar::Script_Devanagari},
		{"sah", QChar::Script_Cyrillic},
		{"sat", QChar::Script_Latin},
		{"sd", QChar::Script_Arabic},
		{"sdh", QChar::Script_Arabic},
		{"se", QChar::Script_Latin},
		{"sg", QChar::Script_Latin},
		{"shi", QChar::Script_Arabic},
		{"si", QChar::Script_Sinhala},
		{"sid", QChar::Script_Latin},
		{"sk", QChar::Script_Latin},
		{"skr", QChar::Script_Arabic},
		{"sl", QChar::Script_Latin},
		{"sm", QChar::Script_Latin},
		{"so", QChar::Script_Latin},
		{"sq", QChar::Script_Latin},
		{"sr", QChar::Script_Cyrillic},
		{"ss", QChar::Script_Latin},
		{"st", QChar::Script_Latin},
		{"su", QChar::Script_Latin},
		{"sus", QChar::Script_Arabic},
		{"sv", QChar::Script_Latin},
		{"sw", QChar::Script_Latin},
		{"swb", QChar::Script_Arabic},
		{"syr", QChar::Script_Arabic},
		{"ta", QChar::Script_Tamil},
		{"te", QChar::Script_Telugu},
		{"tet", QChar::Script_Latin},
		{"tg", QChar::Script_Cyrillic},
		{"th", QChar::Script_Thai},
		{"ti", QChar::Script_Ethiopic},
		{"tig", QChar::Script_Ethiopic},
		{"tk", QChar::Script_Latin},
		{"tkl", QChar::Script_Latin},
		{"tl", QChar::Script_Latin},
		{"tn", QChar::Script_Latin},
		{"to", QChar::Script_Latin},
		{"tpi", QChar::Script_Latin},
		{"tr", QChar::Script_Latin},
		{"trv", QChar::Script_Latin},
		{"ts", QChar::Script_Latin},
		{"tt", QChar::Script_Cyrillic},
		{"ttt", QChar::Script_Arabic},
		{"tvl", QChar::Script_Latin},
		{"tw", QChar::Script_Latin},
		{"ty", QChar::Script_Latin},
		{"tyv", QChar::Script_Cyrillic},
		{"udm", QChar::Script_Cyrillic},
		{"ug", QChar::Script_Arabic},
		{"uk", QChar::Script_Cyrillic},
		{"und", QChar::Script_Latin},
		{"ur", QChar::Script_Arabic},
		{"uz", QChar::Script_Cyrillic},
		{"ve", QChar::Script_Latin},
		{"vi", QChar::Script_Latin},
		{"wal", QChar::Script_Ethiopic},
		{"war", QChar::Script_Latin},
		{"wo", QChar::Script_Latin},
		{"xh", QChar::Script_Latin},
		{"yap", QChar::Script_Latin},
		{"yo", QChar::Script_Latin},
		{"za", QChar::Script_Latin},
		{"zdj", QChar::Script_Arabic},
		{"zh", QChar::Script_Han},
		{"zu", QChar::Script_Latin},
		// Encompassed languages within the Chinese macrolanguage.
		// http://www-01.sil.org/iso639-3/documentation.asp?id=zho
		// http://lists.w3.org/Archives/Public/public-i18n-cjk/2016JulSep/0022.html
		// {"cdo", USCRIPT_SIMPLIFIED_HAN},
		// {"cjy", USCRIPT_SIMPLIFIED_HAN},
		// {"cmn", USCRIPT_SIMPLIFIED_HAN},
		// {"cpx", USCRIPT_SIMPLIFIED_HAN},
		// {"czh", USCRIPT_SIMPLIFIED_HAN},
		// {"czo", USCRIPT_SIMPLIFIED_HAN},
		// {"gan", USCRIPT_SIMPLIFIED_HAN},
		// {"hsn", USCRIPT_SIMPLIFIED_HAN},
		// {"mnp", USCRIPT_SIMPLIFIED_HAN},
		// {"wuu", USCRIPT_SIMPLIFIED_HAN},
		// {"hak", USCRIPT_TRADITIONAL_HAN},
		// {"lzh", USCRIPT_TRADITIONAL_HAN},
		// {"nan", USCRIPT_TRADITIONAL_HAN},
		// {"yue", USCRIPT_TRADITIONAL_HAN},
		// {"zh-cdo", USCRIPT_SIMPLIFIED_HAN},
		// {"zh-cjy", USCRIPT_SIMPLIFIED_HAN},
		// {"zh-cmn", USCRIPT_SIMPLIFIED_HAN},
		// {"zh-cpx", USCRIPT_SIMPLIFIED_HAN},
		// {"zh-czh", USCRIPT_SIMPLIFIED_HAN},
		// {"zh-czo", USCRIPT_SIMPLIFIED_HAN},
		// {"zh-gan", USCRIPT_SIMPLIFIED_HAN},
		// {"zh-hsn", USCRIPT_SIMPLIFIED_HAN},
		// {"zh-mnp", USCRIPT_SIMPLIFIED_HAN},
		// {"zh-wuu", USCRIPT_SIMPLIFIED_HAN},
		// {"zh-hak", USCRIPT_TRADITIONAL_HAN},
		// {"zh-lzh", USCRIPT_TRADITIONAL_HAN},
		// {"zh-nan", USCRIPT_TRADITIONAL_HAN},
		// {"zh-yue", USCRIPT_TRADITIONAL_HAN},
		// // Chinese with regions. Logically, regions should be handled
		// // separately, but this works for the current purposes.
		// {"zh-hk", USCRIPT_TRADITIONAL_HAN},
		// {"zh-mo", USCRIPT_TRADITIONAL_HAN},
		// {"zh-tw", USCRIPT_TRADITIONAL_HAN},
	};

	for (const auto& kv : kLocaleScriptList) {
		if (locale.split('-').first() == kv.subtag) {
			return kv.script;
		}
	}
	return QChar::Script_Common;
}

} // namespace

void SpellCheckerHelper::requestTextCheck(
	QTextDocument &doc,
	std::vector<RangeWord> *misspelledWords,
	std::vector<RangeWord> *correctWords) {
	// We have to calculate the positions of correct words
	// to keep them formatted by user.

	// \b - split the string into an alternating seq of non-word and word tokens
	const auto splitRegexp = QRegExp("_|\\b");

	QTextCursor c(doc.docHandle(), 0);
	const auto from = doc.begin();
	const auto to = doc.end();

	for (auto block = from; block != to; block = block.next()) {
		const auto blockPosition = block.position();
		const auto text = block.text();
		for (const auto &ref : text.splitRef(splitRegexp)) {
			if (ref.trimmed().isEmpty()) {
				continue;
			}

			const auto beginPosition = blockPosition + ref.position();
			const auto endPosition = beginPosition + ref.length();
			const auto range = std::make_pair(beginPosition, endPosition);

			if (!isWordSkippable(ref) &&
				!Platform::Spellchecker::CheckSpelling(ref.toString())) {
				misspelledWords->push_back(range);
			} else {
				correctWords->push_back(range);
			}
		}
	}
}

bool SpellCheckerHelper::isWordSkippable(const QStringRef &word) {
	static auto systemScripts = std::vector<QChar::Script>();
	if (!systemScripts.size()) {
		for (const auto& lang : QLocale::system().uiLanguages()) {
			systemScripts.push_back(LocaleToScriptCode(lang));
		}
	}
	// Find the first letter.
	const auto firstLetter = ranges::find_if(word, [&](QChar c) {
		return c.isLetter();
	});
	if (firstLetter == word.end()) {
		return true;
	}
	const auto wordScript = firstLetter->script();
	if (ranges::find(systemScripts, wordScript) == end(systemScripts)) {
		return true;
	}
	return ranges::find_if(word, [&](QChar c) {
		return (c.script() != wordScript)
			&& (c.unicode() != '_'); // This is not a word separator.
	}) != word.end();
}

void SpellCheckerHelper::fillSuggestionList(const QString &wrongWord,
		std::vector<QString>* optionalSuggestions) {
	Platform::Spellchecker::FillSuggestionList(
		wrongWord,
		optionalSuggestions);
}

bool SpellCheckerHelper::isWordInDictionary(const QString &word) {
	return Platform::Spellchecker::IsWordInDictionary(word);
}

bool SpellCheckerHelper::checkSingleWord(const QString &word) {
	if (!isWordSkippable(&word)) {
		return Platform::Spellchecker::CheckSpelling(word);
	}
	return true;
}

void SpellCheckerHelper::addWord(const QString &word) {
	Platform::Spellchecker::AddWord(word);
}

void SpellCheckerHelper::removeWord(const QString &word) {
	Platform::Spellchecker::RemoveWord(word);
}

void SpellCheckerHelper::ignoreWord(const QString &word) {
	Platform::Spellchecker::IgnoreWord(word);
}

