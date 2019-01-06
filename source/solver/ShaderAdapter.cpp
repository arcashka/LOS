#include "ShaderAdapter.h"

#include <QFile>

ShaderAdapter::ShaderAdapter(const QString & filename, Settings settings)
	: settings(settings)
{
	QFile file(filename);
	file.open(QFile::ReadOnly);
	shaderCode = file.readAll();
}

QByteArray ShaderAdapter::Adapt()
{
	assert(!shaderCode.isEmpty());
	shaderCode.replace("%LOCAL_SIZE%", QString::number(settings.size).toUtf8());
	return shaderCode;
}
