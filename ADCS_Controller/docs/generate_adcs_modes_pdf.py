from __future__ import annotations

from html import escape
from pathlib import Path
import re

from reportlab.lib import colors
from reportlab.lib.enums import TA_CENTER
from reportlab.lib.pagesizes import A4
from reportlab.lib.styles import ParagraphStyle, getSampleStyleSheet
from reportlab.lib.units import mm
from reportlab.pdfbase import pdfmetrics
from reportlab.pdfbase.ttfonts import TTFont
from reportlab.platypus import Paragraph, Preformatted, SimpleDocTemplate, Spacer


ROOT = Path(__file__).resolve().parent
SOURCE = ROOT / "ADCS_Modes_User_Guide_TH.md"
OUTPUT = ROOT / "ADCS_Modes_User_Guide_TH.pdf"

FONT_REGULAR = "LeelawadeeUI"
FONT_BOLD = "LeelawadeeUIBold"
FONT_REGULAR_PATH = Path(r"C:\Windows\Fonts\LeelawUI.ttf")
FONT_BOLD_PATH = Path(r"C:\Windows\Fonts\LeelaUIb.ttf")

INLINE_CODE_RE = re.compile(r"`([^`]+)`")


def register_fonts() -> None:
    pdfmetrics.registerFont(TTFont(FONT_REGULAR, str(FONT_REGULAR_PATH)))
    pdfmetrics.registerFont(TTFont(FONT_BOLD, str(FONT_BOLD_PATH)))


def build_styles() -> dict[str, ParagraphStyle]:
    base = getSampleStyleSheet()
    return {
        "title": ParagraphStyle(
            "ThaiTitle",
            parent=base["Title"],
            fontName=FONT_BOLD,
            fontSize=20,
            leading=24,
            alignment=TA_CENTER,
            textColor=colors.HexColor("#1B2A41"),
            spaceAfter=8,
        ),
        "h1": ParagraphStyle(
            "ThaiH1",
            parent=base["Heading1"],
            fontName=FONT_BOLD,
            fontSize=16,
            leading=20,
            textColor=colors.HexColor("#0B4F6C"),
            spaceBefore=10,
            spaceAfter=4,
        ),
        "h2": ParagraphStyle(
            "ThaiH2",
            parent=base["Heading2"],
            fontName=FONT_BOLD,
            fontSize=13,
            leading=17,
            textColor=colors.HexColor("#145374"),
            spaceBefore=8,
            spaceAfter=2,
        ),
        "body": ParagraphStyle(
            "ThaiBody",
            parent=base["BodyText"],
            fontName=FONT_REGULAR,
            fontSize=10.5,
            leading=15,
            textColor=colors.black,
            spaceAfter=4,
        ),
        "bullet": ParagraphStyle(
            "ThaiBullet",
            parent=base["BodyText"],
            fontName=FONT_REGULAR,
            fontSize=10.5,
            leading=15,
            leftIndent=14,
            firstLineIndent=0,
            bulletIndent=0,
            spaceAfter=2,
        ),
        "code": ParagraphStyle(
            "CodeBlock",
            parent=base["Code"],
            fontName="Courier",
            fontSize=8.5,
            leading=11,
            leftIndent=10,
            rightIndent=10,
            borderPadding=6,
            borderColor=colors.HexColor("#D9E2EC"),
            borderWidth=0.5,
            borderRadius=3,
            backColor=colors.HexColor("#F7FAFC"),
            spaceBefore=2,
            spaceAfter=8,
        ),
    }


def format_inline(text: str) -> str:
    escaped = escape(text)
    return INLINE_CODE_RE.sub(r'<font name="Courier">\1</font>', escaped)


def add_page_number(canvas, doc) -> None:
    canvas.saveState()
    canvas.setFont(FONT_REGULAR, 9)
    canvas.setFillColor(colors.HexColor("#666666"))
    canvas.drawRightString(195 * mm, 10 * mm, f"page {doc.page}")
    canvas.restoreState()


def parse_markdown(text: str, styles: dict[str, ParagraphStyle]):
    story = []
    paragraph_buffer: list[str] = []
    code_lines: list[str] = []
    in_code = False

    def flush_paragraph() -> None:
        nonlocal paragraph_buffer
        if not paragraph_buffer:
            return

        paragraph_text = " ".join(line.strip() for line in paragraph_buffer).strip()
        if paragraph_text:
            story.append(Paragraph(format_inline(paragraph_text), styles["body"]))
        paragraph_buffer = []

    def flush_code() -> None:
        nonlocal code_lines
        if not code_lines:
            return

        story.append(Preformatted("\n".join(code_lines), styles["code"]))
        code_lines = []

    for raw_line in text.splitlines():
        line = raw_line.rstrip()

        if line.startswith("```"):
            flush_paragraph()
            if in_code:
                flush_code()
                in_code = False
            else:
                in_code = True
            continue

        if in_code:
            code_lines.append(line)
            continue

        if not line:
            flush_paragraph()
            story.append(Spacer(1, 2))
            continue

        if line.startswith("# "):
            flush_paragraph()
            story.append(Paragraph(format_inline(line[2:].strip()), styles["title"]))
            story.append(Spacer(1, 4))
            continue

        if line.startswith("## "):
            flush_paragraph()
            story.append(Paragraph(format_inline(line[3:].strip()), styles["h1"]))
            continue

        if line.startswith("### "):
            flush_paragraph()
            story.append(Paragraph(format_inline(line[4:].strip()), styles["h2"]))
            continue

        if line.startswith("- "):
            flush_paragraph()
            story.append(
                Paragraph(
                    format_inline(line[2:].strip()),
                    styles["bullet"],
                    bulletText="-",
                )
            )
            continue

        numbered_match = re.match(r"^(\d+)\.\s+(.*)$", line)
        if numbered_match:
            flush_paragraph()
            marker, content = numbered_match.groups()
            story.append(
                Paragraph(
                    format_inline(content.strip()),
                    styles["bullet"],
                    bulletText=f"{marker}.",
                )
            )
            continue

        paragraph_buffer.append(line)

    flush_paragraph()
    flush_code()
    return story


def main() -> None:
    register_fonts()
    styles = build_styles()
    story = parse_markdown(SOURCE.read_text(encoding="utf-8"), styles)

    doc = SimpleDocTemplate(
        str(OUTPUT),
        pagesize=A4,
        leftMargin=16 * mm,
        rightMargin=16 * mm,
        topMargin=16 * mm,
        bottomMargin=16 * mm,
        title="ADCS Modes User Guide",
        author="OpenAI Codex",
    )
    doc.build(story, onFirstPage=add_page_number, onLaterPages=add_page_number)


if __name__ == "__main__":
    main()
