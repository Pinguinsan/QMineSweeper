#include "PrettyPrinter.hpp"

const char *PrettyPrinter::TERMINAL_ESCAPE_SEQUENCE_BASE{"\033["};
const char *PrettyPrinter::TERMINAL_ESCAPE_SEQUENCE_TAIL{"m"};
const char *PrettyPrinter::DEFAULT_TERMINAL_FOREGROUND_COLOR{"\033[39m"};
const char *PrettyPrinter::DEFAULT_TERMINAL_BACKGROUND_COLOR{"\033[49m"};
const char *PrettyPrinter::DEFAULT_TERMINAL_FONT_ATTRIBUTES{"\033[0m"};
const int RESET_FONT_ATTRIBUTE_OFFSET{20};

const std::vector<ForegroundColor> PrettyPrinter::FOREGROUND_TERMINAL_COLORS_CONTAINER{ForegroundColor::FG_DEFAULT,
                                                                                        ForegroundColor::FG_BLACK,
                                                                                        ForegroundColor::FG_RED,
                                                                                        ForegroundColor::FG_GREEN, 
                                                                                        ForegroundColor::FG_YELLOW, 
                                                                                        ForegroundColor::FG_BLUE, 
                                                                                        ForegroundColor::FG_MAGENTA, 
                                                                                        ForegroundColor::FG_CYAN, 
                                                                                        ForegroundColor::FG_LIGHT_GRAY,
                                                                                        ForegroundColor::FG_DARK_GRAY, 
                                                                                        ForegroundColor::FG_LIGHT_RED, 
                                                                                        ForegroundColor::FG_LIGHT_GREEN, 
                                                                                        ForegroundColor::FG_LIGHT_YELLOW, 
                                                                                        ForegroundColor::FG_LIGHT_BLUE, 
                                                                                        ForegroundColor::FG_LIGHT_MAGENTA, 
                                                                                        ForegroundColor::FG_LIGHT_CYAN, 
                                                                                        ForegroundColor::FG_WHITE};

const std::vector<BackgroundColor> PrettyPrinter::BACKGROUND_TERMINAL_COLORS_CONTAINER{BackgroundColor::BG_DEFAULT,
                                                                                        BackgroundColor::BG_BLACK,
                                                                                        BackgroundColor::BG_RED, 
                                                                                        BackgroundColor::BG_GREEN, 
                                                                                        BackgroundColor::BG_YELLOW,
                                                                                        BackgroundColor::BG_BLUE,
                                                                                        BackgroundColor::BG_MAGENTA,
                                                                                        BackgroundColor::BG_CYAN,
                                                                                        BackgroundColor::BG_LIGHT_GRAY,
                                                                                        BackgroundColor::BG_DARK_GRAY,
                                                                                        BackgroundColor::BG_LIGHT_RED,
                                                                                        BackgroundColor::BG_LIGHT_GREEN,
                                                                                        BackgroundColor::BG_LIGHT_YELLOW,
                                                                                        BackgroundColor::BG_LIGHT_BLUE,
                                                                                        BackgroundColor::BG_LIGHT_MAGENTA,
                                                                                        BackgroundColor::BG_LIGHT_CYAN,
                                                                                        BackgroundColor::BG_WHITE};

PrettyPrinter::PrettyPrinter() :
    PrettyPrinter{ForegroundColor::FG_DEFAULT, BackgroundColor::BG_DEFAULT, &std::cout, FontAttribute::FA_DEFAULT}
{

}

PrettyPrinter::PrettyPrinter(ForegroundColor foregroundColor) :
    PrettyPrinter{foregroundColor, BackgroundColor::BG_DEFAULT, &std::cout, FontAttribute::FA_DEFAULT}
{

}

PrettyPrinter::PrettyPrinter(BackgroundColor backgroundColor) :
    PrettyPrinter{ForegroundColor::FG_DEFAULT, backgroundColor, &std::cout, FontAttribute::FA_DEFAULT}
{

}

PrettyPrinter::PrettyPrinter(std::ostream *outputStream) :
    PrettyPrinter{ForegroundColor::FG_DEFAULT, BackgroundColor::BG_DEFAULT, outputStream, FontAttribute::FA_DEFAULT}
{

}

PrettyPrinter::PrettyPrinter(ForegroundColor foregroundColor, std::ostream *outputStream) :
    PrettyPrinter{foregroundColor, BackgroundColor::BG_DEFAULT, outputStream, FontAttribute::FA_DEFAULT}
{

}

PrettyPrinter::PrettyPrinter(BackgroundColor backgroundColor, std::ostream *outputStream) :
    PrettyPrinter{ForegroundColor::FG_DEFAULT, backgroundColor, outputStream, FontAttribute::FA_DEFAULT}
{

}

PrettyPrinter::PrettyPrinter(ForegroundColor foregroundColor, BackgroundColor backgroundColor, std::ostream *outputStream) :
    PrettyPrinter{foregroundColor, backgroundColor, outputStream, FontAttribute::FA_DEFAULT}
{

}

PrettyPrinter::PrettyPrinter(ForegroundColor foregroundColor, BackgroundColor backgroundColor, FontAttribute fontAttribute) :
    PrettyPrinter{foregroundColor, backgroundColor, &std::cout, fontAttribute}
{

}

PrettyPrinter::PrettyPrinter(ForegroundColor foregroundColor, BackgroundColor backgroundColor, std::ostream *outputStream, FontAttribute fontAttribute) :
    m_foregroundColor{foregroundColor},
    m_backgroundColor{backgroundColor},
    m_outputStream{outputStream},
    m_fontAttributes{fontAttribute},
    m_enabled{true}
{

}

void PrettyPrinter::setEnabled(bool enabled) {
    this->m_enabled = enabled;
}

bool PrettyPrinter::enabled() const {
    return this->m_enabled;
}

void PrettyPrinter::setForegroundColor(ForegroundColor foregroundColor) {
    this->m_foregroundColor = foregroundColor;
}

void PrettyPrinter::setBackgroundColor(BackgroundColor backgroundColor) {
    this->m_backgroundColor = backgroundColor;
}

void PrettyPrinter::setFontAttributes(int fontAttributes) {
    this->m_fontAttributes = fontAttributes;
}

void PrettyPrinter::setOutputStream(std::ostream *outputStream) {
    this->m_outputStream = outputStream;
}

ForegroundColor PrettyPrinter::foregroundColor() const {
    return this->m_foregroundColor;
}

BackgroundColor PrettyPrinter::backgroundColor() const {
    return this->m_backgroundColor;
}

std::ostream *PrettyPrinter::outputStream() {
    return this->m_outputStream;
}

int PrettyPrinter::fontAttributes() const {
    return this->m_fontAttributes;
}

void PrettyPrinter::resetStreamToDefault(std::ostream *oStream) {
    *oStream << PrettyPrinter::DEFAULT_TERMINAL_FOREGROUND_COLOR;
    *oStream << PrettyPrinter::DEFAULT_TERMINAL_BACKGROUND_COLOR;
    *oStream << PrettyPrinter::DEFAULT_TERMINAL_FONT_ATTRIBUTES;
}

void PrettyPrinter::resetBackgroundColor() {
    this->m_backgroundColor = BackgroundColor::BG_DEFAULT;
    *this->m_outputStream << PrettyPrinter::TERMINAL_ESCAPE_SEQUENCE_BASE << this->m_backgroundColor << PrettyPrinter::TERMINAL_ESCAPE_SEQUENCE_TAIL;
}

void PrettyPrinter::resetForegroundColor() {
    this->m_foregroundColor = ForegroundColor::FG_DEFAULT;
    *this->m_outputStream << PrettyPrinter::TERMINAL_ESCAPE_SEQUENCE_BASE << this->m_foregroundColor << PrettyPrinter::TERMINAL_ESCAPE_SEQUENCE_TAIL;
}

void PrettyPrinter::resetFontAttributes() {
    this->m_fontAttributes = FontAttribute::FA_DEFAULT;
    *this->m_outputStream << PrettyPrinter::DEFAULT_TERMINAL_FONT_ATTRIBUTES;
}

std::vector<int> PrettyPrinter::getFontAttributes(int attr) {
    std::vector<int> returnVector;
    if (attr & FontAttribute::FA_DEFAULT) {
        returnVector.push_back(UnderlyingFontAttribute::UFA_DEFAULT);
        return returnVector;
    }
    if (attr & FontAttribute::FA_HIDDEN) {
        returnVector.push_back(UnderlyingFontAttribute::UFA_HIDDEN);
        return returnVector;
    }

    if (attr & FontAttribute::FA_BOLD) {
        returnVector.push_back(UnderlyingFontAttribute::UFA_BOLD);
    }
    if (attr & FontAttribute::FA_DIM) {
        returnVector.push_back(UnderlyingFontAttribute::UFA_DIM);
    }
    if (attr & FontAttribute::FA_UNDERLINED) {
        returnVector.push_back(UnderlyingFontAttribute::UFA_UNDERLINED);
    }
    if (attr & FontAttribute::FA_BLINK) {
        returnVector.push_back(UnderlyingFontAttribute::UFA_BLINK);
    }
    if (attr & FontAttribute::FA_REVERSE) {
        returnVector.push_back(UnderlyingFontAttribute::UFA_REVERSE);
    }
    return returnVector;
}
