local log = pdLog("ofFontSettings")
local canvas = pdCanvas(this, 1)
local currentDir = canvas:getDir() .. "/"
M.settings = ofTrueTypeFontSettings("", 0)

local function isPathRelative(path)
  if path:match"^[~/\\]" or path:match"^%a:[/\\]" then
    return false
  end
    return true
end

function ofelia.bang()
  return M.settings
end

function ofelia.float(f)
  return {f, ofelia.bang()}
end

function ofelia.symbol(s)
  return {s, ofelia.bang()}
end

function ofelia.pointer(p)
  return {p, ofelia.bang()}
end

function ofelia.list(av)
  av[#av+1] = ofelia.bang()
  return av
end

function ofelia.free()
  M.settings = nil
end

function ofelia._set(av)
  local fileName = av[1]
  if isPathRelative(fileName) then
    fileName = currentDir .. fileName
  end
  if #av == 2 then
    M.settings = ofTrueTypeFontSettings(fileName, av[2])
  elseif #av == 6 then
    M.settings = ofTrueTypeFontSettings(fileName, av[2])
    M.settings.antialiased = av[3] ~= 0
    M.settings.contours = av[4] ~= 0
    M.settings.simplifyAmt = av[5]
    M.settings.dpi = av[6]
  else
    log:error("'set' requires 2 or 6 arguments")
  end
end

function ofelia.addRanges(s)
  local alphabet = s
  if alphabet == "Emoji" then
    alphabet = ofAlphabet_Emoji
  elseif alphabet == "Japanese" then
    alphabet = ofAlphabet_Japanese
  elseif alphabet == "Chinese" then
    alphabet = ofAlphabet_Chinese
  elseif alphabet == "Korean" then
    alphabet = ofAlphabet_Korean
  elseif alphabet == "Arabic" then
    alphabet = ofAlphabet_Arabic
  elseif alphabet == "Devanagari" then
    alphabet = ofAlphabet_Devanagari
  elseif alphabet == "Latin" then
    alphabet = ofAlphabet_Latin
  elseif alphabet == "Greek" then
    alphabet = ofAlphabet_Greek
  elseif alphabet == "Cyrillic" then
    alphabet = ofAlphabet_Cyrillic
  else
    log:error("unknown alphabet : " .. alphabet)
    return
  end
  M.settings:addRanges(alphabet)
end

function ofelia.addRange(s)
  local range = s
  if range == "Space" then
    range = ofUnicode.Space
  elseif range == "IdeographicSpace" then
    range = ofUnicode.IdeographicSpace
  elseif range == "Latin" then
    range = ofUnicode.Latin
  elseif range == "Latin1Supplement" then
    range = ofUnicode.Latin1Supplement
  elseif range == "LatinA" then
    range = ofUnicode.LatinA
  elseif range == "Greek" then
    range = ofUnicode.Greek
  elseif range == "Cyrillic" then
    range = ofUnicode.Cyrillic
  elseif range == "Arabic" then
    range = ofUnicode.Arabic
  elseif range == "ArabicSupplement" then
    range = ofUnicode.ArabicSupplement
  elseif range == "ArabicExtendedA" then
    range = ofUnicode.ArabicExtendedA
  elseif range == "Devanagari" then
    range = ofUnicode.Devanagari
  elseif range == "HangulJamo" then
    range = ofUnicode.HangulJamo
  elseif range == "VedicExtensions" then
    range = ofUnicode.VedicExtensions
  elseif range == "LatinExtendedAdditional" then
    range = ofUnicode.LatinExtendedAdditional
  elseif range == "GreekExtended" then
    range = ofUnicode.GreekExtended
  elseif range == "GeneralPunctuation" then
    range = ofUnicode.GeneralPunctuation
  elseif range == "SuperAndSubScripts" then
    range = ofUnicode.SuperAndSubScripts
  elseif range == "CurrencySymbols" then
    range = ofUnicode.CurrencySymbols
  elseif range == "LetterLikeSymbols" then
    range = ofUnicode.LetterLikeSymbols
  elseif range == "NumberForms" then
    range = ofUnicode.NumberForms
  elseif range == "Arrows" then
    range = ofUnicode.Arrows
  elseif range == "MathOperators" then
    range = ofUnicode.MathOperators
  elseif range == "MiscTechnical" then
    range = ofUnicode.MiscTechnical
  elseif range == "BoxDrawing" then
    range = ofUnicode.BoxDrawing
  elseif range == "BlockElement" then
    range = ofUnicode.BlockElement
  elseif range == "GeometricShapes" then
    range = ofUnicode.GeometricShapes
  elseif range == "MiscSymbols" then
    range = ofUnicode.MiscSymbols
  elseif range == "Dingbats" then
    range = ofUnicode.Dingbats
  elseif range == "Hiragana" then
    range = ofUnicode.Hiragana
  elseif range == "Katakana" then
    range = ofUnicode.Katakana
  elseif range == "HangulCompatJamo" then
    range = ofUnicode.HangulCompatJamo
  elseif range == "KatakanaPhoneticExtensions" then
    range = ofUnicode.KatakanaPhoneticExtensions
  elseif range == "CJKLettersAndMonths" then
    range = ofUnicode.CJKLettersAndMonths
  elseif range == "CJKUnified" then
    range = ofUnicode.CJKUnified
  elseif range == "DevanagariExtended" then
    range = ofUnicode.DevanagariExtended
  elseif range == "HangulExtendedA" then
    range = ofUnicode.HangulExtendedA
  elseif range == "HangulSyllables" then
    range = ofUnicode.HangulSyllables
  elseif range == "HangulExtendedB" then
    range = ofUnicode.HangulExtendedB
  elseif range == "AlphabeticPresentationForms" then
    range = ofUnicode.AlphabeticPresentationForms
  elseif range == "ArabicPresFormsA" then
    range = ofUnicode.ArabicPresFormsA
  elseif range == "ArabicPresFormsB" then
    range = ofUnicode.ArabicPresFormsB
  elseif range == "KatakanaHalfAndFullwidthForms" then
    range = ofUnicode.KatakanaHalfAndFullwidthForms
  elseif range == "KanaSupplement" then
    range = ofUnicode.KanaSupplement
  elseif range == "RumiNumericalSymbols" then
    range = ofUnicode.RumiNumericalSymbols
  elseif range == "ArabicMath" then
    range = ofUnicode.ArabicMath
  elseif range == "MiscSymbolsAndPictographs" then
    range = ofUnicode.MiscSymbolsAndPictographs
  elseif range == "Emoticons" then
    range = ofUnicode.Emoticons
  elseif range == "TransportAndMap" then
    range = ofUnicode.TransportAndMap
  elseif range == "EnclosedCharacters" then
    range = ofUnicode.EnclosedCharacters
  elseif range == "Uncategorized" then
    range = ofUnicode.Uncategorized
  elseif range == "AdditionalEmoticons" then
    range = ofUnicode.AdditionalEmoticons
  elseif range == "AdditionalTransportAndMap" then
    range = ofUnicode.AdditionalTransportAndMap
  elseif range == "OtherAdditionalSymbols" then
    range = ofUnicode.OtherAdditionalSymbols
  else
    log:error("unknown range : " .. range)
    return
  end
  M.settings:addRange(range)
end