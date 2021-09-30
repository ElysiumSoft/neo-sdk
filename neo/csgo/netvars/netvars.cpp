#include "netvars.h"
#include "../utilities/xor.h"
#include "../sdk.h"

#include <fstream>
#include <format>

netvar_manager netvars;


std::ofstream ofs;

void DumpAll(std::vector<netvar_manager::netvar_table> db)
{
}

    void netvar_manager::init() {
        ofs.open("C:\\Users\\lewis\\OneDrive\\Desktop\\CSGO-NETVARS.txt", std::ios::out | std::ios::trunc);
        database.clear();
        for (auto clientclass = csgo::client->get_client_classes();
            clientclass != nullptr;
            clientclass = clientclass->next_ptr) {
            if (clientclass->recvtable_ptr) {
                database.emplace_back(load_table(clientclass->recvtable_ptr));
            }
        }

        DumpAll(database);
    }

    netvar_manager::netvar_table netvar_manager::load_table(recv_table* recv_table) {
        utils::info_print("^yBUILDING^! & ^cDUMPING^! NETVAR DATATABLES");
        auto table = netvar_table{};
        table.offset = 0;
        std::string szOffset = {};
        for (int i = 0; i < table.offset; i++)
            szOffset += "\t";
        table.name = recv_table->table_name;
        ofs << szOffset <<  "TABLE: " << table.name << "{\n";
        for (auto i = 0; i < recv_table->props_count; ++i) {
            auto prop = &recv_table->props[i];
            
            if (!prop || isdigit(prop->prop_name[0]))
                continue;
            if (strcmp(xor("baseclass"), prop->prop_name) == 0)
                continue;
            if (prop->prop_type == _data_table && prop->data_table) {
                table.child_tables.emplace_back(load_table(prop->data_table));
                table.child_tables.back().offset = prop->offset;
                table.child_tables.back().prop = prop;
                ofs << szOffset << "\tprop[childDataTable] " << prop->prop_name << " ~=~> " << prop->data_table->table_name << "\n";
            }
            else {
                table.child_props.emplace_back(prop);
                ofs << szOffset << "\tprop[" << prop->prop_type << "] " << prop->prop_name << " @ " << prop->offset << "\n";
            }
        }
        return table;
        utils::info_print("^gFINISHED BUILDING + DUMPING DATATABLES :)");
    }

    uint32_t netvar_manager::get_offset(const std::string& tableName, const std::string& propName) {
        auto result = 0u;
        for (const auto& table : database) {
            if (table.name == tableName) {
                result = get_offset(table, propName);
                if (result != 0)
                    return result;
            }
        }
        return 0;
    }

    uint32_t netvar_manager::get_offset(const netvar_table& table, const std::string& propName) {
        for (const auto& prop : table.child_props) {
            if (strncmp(prop->prop_name, propName.data(), propName.size()) == 0) {
                return table.offset + prop->offset;
            }
        }
        for (const auto& child : table.child_tables) {
            auto prop_offset = get_offset(child, propName);
            if (prop_offset != 0)
                return table.offset + prop_offset;
        }
        for (const auto& child : table.child_tables) {
            if (strncmp(child.prop->prop_name, propName.data(), propName.size()) == 0) {
                return table.offset + child.offset;
            }
        }
        return 0;
    }