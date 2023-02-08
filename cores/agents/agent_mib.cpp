/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "agent_mib.h"

Agent_Mib::Agent_Mib()
{
    createHead();
    m_module = "mpro";
}

Agent_Mib *Agent_Mib::bulid()
{
    static Agent_Mib* sington = nullptr;
    if(sington == nullptr) {
        sington = new Agent_Mib();
    }
    return sington;
}

void Agent_Mib::createHead()
{
    m_mib = "M-PRO-PDU-MIB DEFINITIONS ::= BEGIN \n\n"
            "--- ----------------------------------\n"
            "--- Imports\n"
            "--- ----------------------------------\n"
            "IMPORTS\n"
            "   MODULE-IDENTITY, OBJECT-TYPE, enterprises,\n"
            "   TimeTicks, Counter, Integer32, Gauge32,\n"
            "   NOTIFICATION-TYPE                       FROM SNMPv2-SMI\n"
            "   DisplayString                           FROM SNMPv2-TC\n"
            "   sysUpTime                              FROM RFC1213-MIB\n"
            "   TRAP-TYPE                              FROM RFC-1215;\n\n"

            "--- ----------------------------------\n"
            "--- pdu MODULE-IDENTITY\n"
            "---    LAST-UPDATED \"202303040000Z\"		-- March 4th, 2023\n"
            "---    ORGANIZATION \"pdu-snmp-subagent\"\n"
            "---    DESCRIPTION  \"Top-level of the SNMP pdu MIB tree.\"\n"
            "---    REVISION     \"202303040000Z\"\n"
            "---    DESCRIPTION  \"Initial revision.\"\n"
            "--- ----------------------------------\n"
            "pdu OBJECT IDENTIFIER ::= { enterprises 30966}\n\n"

            "--- ----------------------------------\n"
            "--- mpro\n"
            "--- ----------------------------------\n"
            "mpro OBJECT IDENTIFIER ::= { pdu 11 }\n\n";
    //    qDebug().noquote() << m_mib ;
}

void Agent_Mib::appendModule(const QString &name, int oid)
{
    QString fmd = "--- ----------------------------------\n"
                  "--- %4\n"
                  "--- ----------------------------------\n"
                  "%1 OBJECT IDENTIFIER ::= { %2 %3 }\n\n";
    QString description = name; description.replace("-", " ");
    QString str = fmd.arg(name, m_module, QString::number(oid), description);
    if(!m_mib.contains(name+" OBJECT IDENTIFIER")) {m_mib.append(str); qDebug().noquote() << str;}
    m_module = name;
}

void Agent_Mib::appendNode(const sOidIt &it, uint oid)
{
    QString fmd = "%1 OBJECT-TYPE\n"
                  "  SYNTAX  	%2\n"
                  "  MAX-ACCESS  %3\n"
                  "  STATUS      %4\n"
                  "  DESCRIPTION \"%5\"\n"
                  "  ::= { %6 %7 }\n";
    QString objectname = it.name, status="current"; appendModule(it);
    QString syntax = "Integer32"; if(it.str) syntax = "DisplayString";
    QString access = "read-only"; if(it.enWrited) access = "read-write";
    QString description = it.name; description.replace("-", " ");
    QString parent  = m_module, number = QString::number(oid);
    QString str = fmd.arg(objectname, syntax, access, status, description, parent, number);
    m_mib.append(str); //qDebug().noquote() << str ;
}


void Agent_Mib::appendModule(const sOidIt &it)
{
    QStringList os = it.oid.split(".");
    QStringList ls = it.name.split("-");
    QString name = "pdu-"+ls.at(1)+"-";
    appendModule(name+"dev", it.fieldId);
    qDebug() << os << ls;

    if(os.size()>1) appendModule("pdu-"+ls.at(1)+"-"+ls.at(2), os.at(0).toInt());


    if(os.size()>2) appendModule("pdu-"+ls.at(1)+"-"+ls.at(2)+"-"+ls.at(3), os.at(1).toInt());
    if(os.size()>3) appendModule("pdu-"+ls.at(1)+"-"+ls.at(2)+"-"+ls.at(3)+"-"+ls.at(4), os.at(2).toInt());
    if(os.size()>4) appendModule("pdu-"+ls.at(1)+"-"+ls.at(2)+"-"+ls.at(3)+"-"+ls.at(4)+"-"+ls.at(5), os.at(3).toInt());
}

void Agent_Mib::save()
{
    QString fn = "/usr/data/clever/doc/snmp.mib";
    m_mib.append("\nEND\n"); QFile file(fn);
    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file.write(m_mib.toLatin1());
    } file.close();
}
