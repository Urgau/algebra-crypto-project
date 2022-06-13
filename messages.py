#!/usr/bin/env python3

COMMANDS = [
    ("Identify", [
        ("babel::User::Id", "UserId")
    ]),
    ("Update", [
        ("babel::User::Name", "Username"),
        ("babel::User::Avatar", "Avatar"),
        ("babel::User::Visibility", "Visibility")
    ]),
    ("Users", [
        ("std::size_t", "MaxUsers")
    ]),
    ("Call", [
        ("babel::User::Id", "UserId"),
    ]),
    ("HangUp", [
        ("babel::User::Id", "UserId")
    ]),
    ("Message", [
        ("babel::User::Id", "UserId"),
        ("babel::Message::Body", "Body")
    ])
]

EVENTS = [
    ("Self", [
        ("babel::User::Id", "UserId"),
        ("babel::User::Name", "Username"),
        ("babel::User::Avatar", "Avatar"),
        ("babel::User::Visibility", "Visibility")
    ]),
    ("User", [
        ("babel::User::Id", "UserId"),
        ("babel::User::Name", "Username"),
        ("babel::User::Avatar", "Avatar"),
        ("babel::User::Status", "Status"),
        ("babel::User::Visibility", "Visibility")
    ]),
    ("Call", [
        ("babel::User::Id", "FromUserId"),
        ("babel::User::Id", "ToUserId"),
        ("babel::net::Ip", "Ip"),
        ("babel::net::Port", "Port"),
        ("std::string", "Password")
    ]),
    ("HangUp", [
        ("babel::User::Id", "FromUserId"),
        ("babel::User::Id", "ToUserId")
    ]),
    ("Message", [
        ("babel::User::Id", "FromUserId"),
        ("babel::User::Id", "ToUserId"),
        ("babel::Message::Timestamp", "Timestamp"),
        ("babel::Message::Body", "Body")
    ]),
    ("Error", [
        ("std::string", "Message")
    ])
]

def toVariableName(name):
    return ("{}{}".format(name[:1].lower(), name[1:]))

def writeCommonMessageHeader(dirName, mName, mInputs):
    with open("common/include/{}/{}.hpp".format(dirName, mName), "wt") as file:
        file.write("""/*
** EPITECH PROJECT, 2020
** common/include/{}/{}
** File description:
** DO NOT EDIT THIS FILE MANUALLY
*/

#ifndef {}_HPP_
#define {}_HPP_

#include "AMessage.hpp"
""".format(dirName, mName, mName.upper(), mName.upper()))

        for (type, name) in mInputs:
            if type.startswith("babel::User"):
                file.write("""#include "User.hpp"\n""")
            elif type.startswith("babel::net"):
                file.write("""#include "Net.hpp"\n""")
            elif type.startswith("babel::Message"):
                file.write("""#include "Message.hpp"\n""")
            elif type == "std::string":
                file.write("""#include <string>\n""")

        file.write("""
namespace babel {{
    namespace {} {{
        /// \\brief {}
        class {} : public AMessage {{
            public:
                /// \\brief Dummy constructor (for serialize)
                {}();

                /// \\brief Constructor of {} class
                {}({});

                /// \\brief Deconstructor of {}
                ~{}() = default;
""".format(dirName, mName, mName, mName, mName, mName, ','.join("{} {}".format(type, toVariableName(name)) for (type, name) in mInputs),
                    mName, mName))
        for (type, name) in mInputs:
            file.write("""
                /// \\brief Getter for {}
                {} get{}() const;
""".format(name, type, name));
        file.write("""
                template<class Archive>
                void serialize(Archive &ar, const unsigned int)
                {{""".format(mName))
        for (type, name) in mInputs:
            file.write("""
                    ar & _{};""".format(toVariableName(name)));
        file.write("""
                }

            private:""");
        for (type, name) in mInputs:
            file.write("""
                {} _{};""".format(type, toVariableName(name)));
        file.write("""
        };
    }
}

#endif
""");

def writeCommonMessageCode(dirName, mName, mInputs):
    with open("common/src/{}/{}.cpp".format(dirName, mName), "w") as file:
        file.write("""/*
** EPITECH PROJECT, 2020
** common/include/{}/{}
** File description:
** DO NOT EDIT THIS FILE MANUALLY
*/

#include \"{}/{}.hpp\"

namespace babel {{
    namespace {} {{""".format(dirName, mName, dirName, mName, dirName))
        file.write("""
        {}::{}() : AMessage(\"{}\")
        {{ /* ignore */ }}
""".format(mName, mName, mName))

        file.write("""
        {}::{}({}) :
            AMessage(\"{}\")""".format(mName, mName,
            ','.join("{} {}".format(type, toVariableName(name)) for (type, name) in mInputs), mName))

        for (type, name) in mInputs:
            file.write(""", _{}({})""".format(toVariableName(name), toVariableName(name)))

        file.write("""
        { /* ignore */ }""")

        for (type, name) in mInputs:
            file.write("""

        {} {}::get{}() const
        {{
            return (this->_{});
        }}""".format(type, mName, name, toVariableName(name)))

        file.write("""
    }
}
""");

def writeMainFromTemplate(dirName, mClass, mNamespace, mNames, mOtherNamespace, mOtherNames):
    def writeIncludes(file):
        file.write("""#include <map>
#include <sstream>
#include <boost/serialization/string.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>""")

        def writeIncludesFromList(namespace, names):
            for name in names:
                file.write("""
#include "{}/{}.hpp\"""".format(namespace, name))

        writeIncludesFromList(mNamespace, mNames)
        writeIncludesFromList(mOtherNamespace, mOtherNames)
        file.write("\n")

    def writeFunctions(file):
        for mName in mNames:
            file.write("""
            /// \\brief Function which handle {}
            void handle{}(SessionPtr, const {}::{} &);
""".format(mName, mName, mNamespace, mName))

        file.write("""
            template<class M, void (babel::{}::*f)(SessionPtr, const M &)>
            void deserializeCallHandler(SessionPtr session, std::istream &input)
            {{
                M a;

                try {{
                    boost::archive::text_iarchive ia(input);
                    ia >> a;
                }} catch (const std::exception &ex) {{
                    throw Error("Failed to deserialize input", "deserializeCallHandler");
                }}

                (*this.*f)(session, a);
            }}
""".format(mClass))

        file.write("""
            template<class M>
            void serializeAndSend(SessionPtr session, const M &a)
            {{
                std::stringbuf stringbuf;

                {{
                    std::ostream output(&stringbuf);
                    output << a.getMessageName() << " ";
                    boost::archive::text_oarchive oa(output);
                    oa << a;
                }}

                session->deliver(stringbuf.str());
            }}
""".format())


        file.write("""
            /// \\brief Map that store the name of the handler and a wrapper function
            /// that parse the serialize data and call the real handler function
            const std::map<std::string, void (babel::{}::*)(SessionPtr, std::istream &)> HANDLERS = {{""".format(mClass))

        for mName in mNames:
            file.write("""
                {{ "{}", &babel::{}::deserializeCallHandler
                    <babel::{}::{}, &babel::{}::handle{}> }},""".format(mName, mClass, mNamespace, mName, mClass, mName))
        file.write("""
            }};""".format())

    def writeSignals(file):
        for mName in mNames:
            file.write("""
            /// \\brief Signal for handling {}
            void handle{}(const {}::{} &);
""".format(mName, mName, mNamespace, mName))

        file.write("""
        public:
            /// \\brief Serialize an input and send it to the corresponding socket
            template<class M>
            void serializeAndSend(const M &a)
            {{
                std::stringbuf stringbuf;

                {{
                    std::ostream output(&stringbuf);
                    output << a.getMessageName() << " ";
                    boost::archive::text_oarchive oa(output);
                    oa << a;
                }}

                write(stringbuf.str().c_str());
            }}
""".format())

        file.write("""
        private:
            /// \\brief Deserialize an input and send the corresponding signal
            template<class M, void (babel::{}::*f)(const M &)>
            void deserializeAndEmitSignal(std::istream &input)
            {{
                M a;

                try {{
                    boost::archive::text_iarchive ia(input);
                    ia >> a;
                }} catch (const std::exception &ex) {{
                    throw Error("Failed to deserialize input", "deserializeAndEmitSignal");
                }}

                emit (*this.*f)(a);
            }}
""".format(mClass))

        file.write("""
            /// \\brief Map that store the name of the handler and a wrapper function
            /// that parse the serialize data and call the real handler function
            const std::map<std::string, void (babel::{}::*)(std::istream &)> HANDLERS = {{""".format(mClass))

        for mName in mNames:
            file.write("""
                {{ "{}", &babel::{}::deserializeAndEmitSignal
                    <babel::{}::{}, &babel::{}::handle{}> }},""".format(mName, mClass, mNamespace, mName, mClass, mName))
        file.write("""
            }};""".format())

    with open("{}/templates/{}.hpp".format(dirName, mClass), "r") as rFile:
        with open("{}/include/{}.hpp".format(dirName, mClass), "w") as wFile:
            for line in rFile:
                if line == "##HANDLERS_HEADERS##\n":
                    writeIncludes(wFile)
                elif line == "##HANDLERS_FUNCTIONS##\n":
                    writeFunctions(wFile)
                elif line == "##HANDLERS_SIGNALS##\n":
                    writeSignals(wFile)
                else:
                    wFile.write(line)

eventsNames = []
commandsNames = []

for (cName, cInputs) in COMMANDS:
    fullName = "{}Command".format(cName)
    commandsNames.append(fullName)
    writeCommonMessageHeader("commands", fullName, cInputs)
    writeCommonMessageCode("commands", fullName, cInputs)

for (eName, eInputs) in EVENTS:
    fullName = "{}Event".format(eName)
    eventsNames.append(fullName)
    writeCommonMessageHeader("events", fullName, eInputs)
    writeCommonMessageCode("events", fullName, eInputs)

writeMainFromTemplate("server", "Instance",
    "commands", commandsNames, "events", eventsNames)
writeMainFromTemplate("client", "ClientConnection",
    "events", eventsNames, "commands", commandsNames)
