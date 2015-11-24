#include <stdio.h>
#include <iostream>
#include <QDir>
#include <QDebug>
#include <QCoreApplication>
#include <QFile>
#include <QStringList>

static void help()
{
    printf( "This is a word find and remove in multiple files sample.\n"
        "Usage: Find and Remove\n"
		"     -d <directory>			# The directory that contains the files to process. Example: -d C:/TEMP \n"
		"     -e <extensions>			# The list of extensions. Example: -e *.h *.cpp *.log\n"
		"     -input <word to remove>	# The word to find and remove from the files in the directories\n"
        "\n" );
}

/**
 * @brief readFindWriteFile - Read the file, find the word and write the file.
 * @param fileName - Filename.
 * @param word - Keyword.
 */
void readFindWriteFile(QString fileName, QString keyword  )
{
    QFile file(fileName);

	if(file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QString s;
		QTextStream textStream(&file);

        while( !textStream.atEnd() )
        {
            QString line = textStream.readLine();
			if (!line.contains(keyword)) {
				s.append(line + "\n");
			}

        }

		file.resize(0);
		textStream << s;
		file.close();
    }

}

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	static QString directory = "";
	static QString word = "";
	static QStringList filters;

	if (argc < 2)
    {
        help();
        return 0;
    }

	for(int i = 1; i < argc; i++ )
    {
		QString parameter = argv[i];
		if (parameter.compare("-d", Qt::CaseSensitive) == 0)
		{
			i++;
			QString parameterValue = argv[i];
			if (parameterValue.isEmpty() || parameterValue.isNull())
				return fprintf( stderr, "Please set a directory.\n" ), -1;
			else
				directory = parameterValue;
		} 
		else if (parameter.compare("-e", Qt::CaseSensitive) == 0)
		{
			i++;
			QString parameterValue = argv[i];
			if(parameterValue.isEmpty() || parameterValue.isNull())
				return fprintf( stderr, "Please set a list of file extensions. Example: *.cpp *.h *..\n" ), -1;
			else if (!parameterValue.contains("*."))
				return fprintf( stderr, "Please set a list of file extensions. Example: *.cpp *.h *..\n" ), -1;
			else {
				while (!parameterValue.contains("-")) {
					filters.push_back(parameterValue);
					i++;
					parameterValue = argv[i];
				}
				i--;
			}
		}
        else if (parameter.compare("-input", Qt::CaseSensitive) == 0)
        {
			i++;
            QString parameterValue = argv[i];
            if (parameterValue.isEmpty() || parameterValue.isNull())
				return fprintf( stderr, "Please set a word.\n" ), -1;
			else
				word = parameterValue;
        }
        else
            return fprintf( stderr, "Unknown option %s", parameter ), -1;
    }

    QDir dir(directory);

    //QStringList filters;
    //filters << "*.h" << "*.cpp";

    dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList(filters, QDir::Files|QDir::NoDotAndDotDot);
    std::cout << "     Bytes Filename" << std::endl;
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        std::cout << qPrintable(QString("%1 %2").arg(fileInfo.size(), 10)
                                                .arg(fileInfo.fileName()));
        std::cout << std::endl;

        QString fileAbsolutePath = fileInfo.absoluteFilePath();

		readFindWriteFile(fileAbsolutePath,word);
	}

	return a.exec();
}
