#include "wsconfigport.h"

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QtDebug>

WsConfigPort::WsConfigPort()
{
}

WsConfigPort::~WsConfigPort()
{

}

QString WsConfigPort::registerName() const
{
    return "WsConfigPort";
}

Plugin::PluginType WsConfigPort::pluginType() const
{
    return Plugin::PluginType::CONFIGPORT;
}

void WsConfigPort::save()
{
    QFile fout(this->file_path);
    if(!fout.open(QIODevice::WriteOnly | QIODevice::Text)){
        schedule_core->writeError(this, "配置文件保存错误",
                         QString("指定配置文件%1保存过程中，无法打开").arg(file_path));
        return;
    }
    QTextStream tout(&fout);
    tout << this->configure_doc.toString(4);
    tout.flush();
    fout.flush();
    fout.close();
}

Plugin::ConfigPort *WsConfigPort::newConfigPort(Plugin::Schedule *core)
{
    auto config = new WsConfigPort();
    config->schedule_core = core;
    config->file_path = "";
    auto ppc = config->configure_doc.createProcessingInstruction("xml", "version='1.0' ");
    config->configure_doc.appendChild(ppc);

    auto root = config->configure_doc.createElement("config-list");
    config->configure_doc.appendChild(root);

    return config;
}

QString WsConfigPort::filePath() const{
    return file_path;
}

void WsConfigPort::openFile(const QString &file_path)
{
    QFile cfg(file_path);
    if(!cfg.exists()){
        schedule_core->writeError(this, "配置文件读写错误", "指定配置文件不存在");
        return;
    }
    if(!cfg.open(QIODevice::ReadOnly | QIODevice::Text)){
        schedule_core->writeError(this, "配置文件读取错误", "指定配置文件无法打开");
        return;
    }

    this->file_path = file_path;
    if(!this->configure_doc.setContent(&cfg, false)){
        schedule_core->writeError(this, "配置文件解析错误", "指定配置文件无法解析");
        return;
    }
    cfg.close();
}

void WsConfigPort::newFile(const QString &project_eroot_path)
{
    QDir pjt(project_eroot_path);
    this->file_path = pjt.absoluteFilePath("config.config");

    QFile cfg(file_path);
    if(!cfg.open(QIODevice::Text | QIODevice::WriteOnly)){
        schedule_core->writeError(this, "配置文件打开错误", QString("新建文件无法打开错误%1").arg(file_path));
        return;
    }

    QTextStream tout(&cfg);
    tout << this->configure_doc.toString(4);

    tout.flush();
    cfg.flush();
    cfg.close();
}

/*
 * <?xml version='1.0' ?>
 * <config-list>
 *  <plugin-config name="register-name">
 *   <value key="key-name">value<value>
 *   <list name="listname">
 *    <item>list-value</item>
 *   </list>
 *   <map name="mapname">
 *    <item key="key-name">value</item>
 *   </map>
 *  </plugin-config>
 * </config-list>
 */

void WsConfigPort::setValue(const Base *base, const QString &key, const QString &val)
{
    auto config_list = this->configure_doc.documentElement();
    auto pnode = locateChildElement(config_list, "plugin-config",
                                    "name", base->registerName());

    if(pnode.isNull()){
        pnode = this->configure_doc.createElement("plugin-config");
        pnode.setAttribute("name", base->registerName());
        config_list.appendChild(pnode);
    }
    auto target_node = locateChildElement(pnode, "value", "key", key);
    if(!target_node.isNull()){
        pnode.removeChild(target_node);
    }

    target_node = this->configure_doc.createElement("value");
    target_node.setAttribute("key", key);
    target_node.appendChild(this->configure_doc.createTextNode(val));
    pnode.appendChild(target_node);
}

QString WsConfigPort::getValue(const Plugin::Base *base, const QString &key) const
{
    auto config_list = this->configure_doc.documentElement();
    auto pnode = locateChildElement(config_list, "plugin-config",
                                    "name", base->registerName());
    if(pnode.isNull())
        return "";

    auto target = locateChildElement(pnode, "value", "key", key);
    if(target.isNull())
        return "";

    return target.text();
}
void WsConfigPort::setList(const Plugin::Base *base, const QString &name, const QList<QString> &list)
{
    auto config_list = this->configure_doc.documentElement();
    auto pnode = locateChildElement(config_list, "plugin-config",
                                    "name", base->registerName());
    if(pnode.isNull()){
        pnode = this->configure_doc.createElement("plugin-config");
        pnode.setAttribute("name", base->registerName());
        config_list.appendChild(pnode);
    }

    auto target = locateChildElement(pnode, "list", "name", name);
    if(!target.isNull()){
        pnode.removeChild(target);
    }
    target = this->configure_doc.createElement("list");
    target.setAttribute("name", name);
    pnode.appendChild(target);
    for(auto &item : list){
        auto itemnode = this->configure_doc.createElement("item");
        itemnode.appendChild(this->configure_doc.createTextNode(item));
        target.appendChild(itemnode);
    }
}

QList<QString> WsConfigPort::getList(const Plugin::Base *base, const QString &name) const
{
    QList<QString> list;
    auto config_list = this->configure_doc.documentElement();
    auto pnode = locateChildElement(config_list, "plugin-config",
                                    "name", base->registerName());
    if(pnode.isNull())
        return list;

    auto target = locateChildElement(pnode, "list", "name", name);
    if(target.isNull())
        return list;

    auto childs = target.childNodes();
    for(auto index=0; index < childs.size(); ++index){
        list.append(childs.at(index).toElement().text());
    }

    return list;
}
void WsConfigPort::setMap(const Plugin::Base *base, const QString &name, const QHash<QString, QString> &map)
{
    auto config_list = this->configure_doc.documentElement();
    auto pnode = locateChildElement(config_list, "plugin-config",
                                    "name", base->registerName());
    if(pnode.isNull()){
        pnode = this->configure_doc.createElement("plugin-config");
        pnode.setAttribute("name", base->registerName());
        config_list.appendChild(pnode);
    }

    auto target = locateChildElement(pnode, "map", "name", name);
    if(!target.isNull()){
        pnode.removeChild(target);
    }
    target = this->configure_doc.createElement("map");
    target.setAttribute("name", name);
    pnode.appendChild(target);

    auto keys = map.keys();
    for(auto &key : keys){
        auto node = this->configure_doc.createElement("item");
        node.setAttribute("key", key);
        target.appendChild(node);

        node.appendChild(this->configure_doc.createTextNode(map.value(key)));
    }
}

QHash<QString, QString> WsConfigPort::getMap(const Plugin::Base *base, const QString &name) const
{
    QHash<QString,QString> map;

    auto config_list = this->configure_doc.documentElement();
    auto pnode = locateChildElement(config_list, "plugin-config",
                                    "name", base->registerName());
    if(pnode.isNull())
        return map;

    auto target = locateChildElement(pnode, "map", "name", name);
    if(target.isNull())
        return map;

    auto children = target.childNodes();
    for (auto index=0; index<children.size(); ++index) {
        auto elm = children.at(index).toElement();
        auto value = elm.text();
        map.insert(elm.attribute("key"), value);
    }

    return map;
}

QDomElement WsConfigPort::locateChildElement(QDomElement &node, const QString &tag_name,
                                             const QString &attr_name, const QString &target_value) const
{
    QDomNodeList children;
    if(tag_name == "")
        children = node.childNodes();
    else
        children = node.elementsByTagName(tag_name);

    for(int i=0; i<children.size(); ++i){
        auto elm = children.at(i).toElement();
        if(elm.attribute(attr_name) == target_value)
            return elm;
    }

    return QDomElement();
}


