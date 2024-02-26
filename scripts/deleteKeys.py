def isSingleLineWhiteListed(line: str) -> bool:
    tag, colon, value = line.partition(":")
    if colon == "":
        return False
    return tag in ["title", "date"]


def listOpens(line: str) -> bool:
    return line in ["categories:\n", "tags:\n"]


def listContinues(line: str) -> bool:
    return line.startswith("- ")


def ReadFileAndSanitizeHeader(fileStr: str) -> list:
    FRONT_MATTER_LINE = "---\n"

    lines = []
    with open(fileStr, "r") as file:
        if file.readline() == FRONT_MATTER_LINE:
            lines = [FRONT_MATTER_LINE]
            parsingList = False
            while True:
                line = file.readline()
                if line == "":
                    raise "Front matter header not closed!"
                if line == FRONT_MATTER_LINE:
                    lines.append(FRONT_MATTER_LINE)
                    break

                if parsingList:
                    if listContinues(line):
                        lines.append(line)
                        continue
                    else:
                        parsingList = False

                if isSingleLineWhiteListed(line):
                    lines.append(line)

                if listOpens(line):
                    parsingList = True
                    lines.append(line)

            lines += file.readlines()
    return lines


def processFile(fileStr: str) -> None:
    lines = ReadFileAndSanitizeHeader(fileStr)
    if not lines:
        return
    with open(fileStr, "w") as file:
        file.write("".join(lines))


def loopFiles():
    import os

    FOLDER = "_posts/old/"
    for _, _, files in os.walk(FOLDER):
        for file in files:
            processFile(FOLDER + file)
        break


if __name__ == "__main__":
    loopFiles()
