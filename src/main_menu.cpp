#include "main_menu.hpp"
#include "audio.hpp"
#include "character_making.hpp"
#include "config/config.hpp"
#include "ctrl_file.hpp"
#include "i18n.hpp"
#include "input.hpp"
#include "keybind/keybind.hpp"
#include "macro.hpp"
#include "main_menu.hpp"
#include "menu.hpp"
#include "random.hpp"
#include "ui.hpp"
#include "variables.hpp"
#include "version.hpp"

namespace elona
{

MainMenuResult main_title_menu()
{
    mode = 10;
    lomiaseaster = 0;
    play_music("core.mcOpening");
    cs = 0;
    cs_bk = -1;
    keyrange = 6;
    pagesize = 0;
    load_background_variants(2);
    gsel(4);
    gmode(0);
    pos(0, 0);
    picload(filesystem::dir::graphic() / u8"title.bmp", 1);
    gcopy(4, 0, 0, 800, 600, windoww, windowh);
    gmode(2);
    font(13 - en * 2);
    color(255, 255, 255);
    pos(20, 20);
    mes(u8"Elona version 1.22  Developed by Noa"s);
    color(0, 0, 0);
    color(255, 255, 255);
    pos(20, 38);
    mes(u8"  Variant foobar version "s + latest_version.short_string()
        + u8"  Developed by KI");
    color(0, 0, 0);
    if (jp)
    {
        color(255, 255, 255);
        pos(20, 56);
        mes(u8"Contributor MSL / View the credits for more"s);
        color(0, 0, 0);
    }
    else
    {
        color(255, 255, 255);
        pos(20, 56);
        mes(u8"Contributor f1r3fly, Sunstrike, Schmidt, Elvenspirit / View the credits for more"s);
        pos(20, 76);
        mes(u8"Omake/OO translator: Doorknob");
        pos(20, 96);
        mes(u8"Cutscenes translator: AnnaBannana");
        color(0, 0, 0);
    }
    if (jp)
    {
        s(0) = u8"冒険の道標"s;
        s(1) = strhint1;
    }
    else
    {
        s(0) = u8"Starting Menu"s;
        s(1) = strhint1;
    }
    windowshadow = 1;
    display_window(80, winposy(308, 1), 320, 320);
    cmbg = 4;
    x = ww / 5 * 4;
    y = wh - 80;
    pos(wx + 160, wy + wh / 2);
    gmode(4, 50);
    gcopy_c(2, cmbg / 2 * 180, cmbg % 2 * 300, 180, 300, x, y);
    gmode(2);
    if (jp)
    {
        s(0) = u8"Restore an adventurer"s;
        s(1) = u8"冒険を再開する"s;
        s(2) = u8"Generate an adventurer"s;
        s(3) = u8"新しい冒険者を作成する"s;
        s(4) = u8"Incarnate an adventurer"s;
        s(5) = u8"冒険者の引継ぎ"s;
        s(6) = u8"View the homepage"s;
        s(7) = u8"ホームページを開く"s;
        s(8) = u8"Configure"s;
        s(9) = u8"設定の変更"s;
        s(10) = u8"Exit"s;
        s(11) = u8"終了"s;
    }
    if (en)
    {
        s(0) = u8"Restore an Adventurer"s;
        s(1) = u8"Generate an Adventurer"s;
        s(2) = u8"Incarnate an Adventurer"s;
        s(3) = u8"View the Homepage"s;
        s(4) = u8"Options"s;
        s(5) = u8"Exit"s;
    }
    gsel(3);
    pos(960, 96);
    picload(filesystem::dir::graphic() / u8"deco_title.bmp", 1);
    gsel(0);
    gmode(0);
    pos(0, 0);
    gcopy(4, 0, 0, windoww, windowh);
    gmode(2);

    while (1)
    {
        if (Config::instance().autonumlock)
        {
            snail::Input::instance().disable_numlock();
        }

        gmode(0);
        pos(0, 0);
        gcopy(4, 0, 0, windoww, windowh);
        gmode(2);
        cs_listbk();
        for (int cnt = 0; cnt < 6; ++cnt)
        {
            x = wx + 40;
            y = cnt * 35 + wy + 50;
            display_customkey(key_select(cnt), x, y);
            if (jp)
            {
                font(11 - en * 2);
                pos(x + 40, y - 4);
                mes(s(cnt * 2));
                font(13 - en * 2);
                cs_list(cs == cnt, s(cnt * 2 + 1), x + 40, y + 8);
            }
            else
            {
                font(14 - en * 2);
                cs_list(cs == cnt, s(cnt), x + 40, y + 1);
            }
        }
        cs_bk = cs;
        redraw();

        int index{};
        cursor_check_ex(index);

        if (index == 1)
        {
            snd("core.ok1");
            geneuse = "";
            return MainMenuResult::main_menu_new_game;
        }
        if (index == 0)
        {
            snd("core.ok1");
            return MainMenuResult::main_menu_continue;
        }
        if (index == 2)
        {
            snd("core.ok1");
            return MainMenuResult::main_menu_incarnate;
        }
        if (index == 3)
        {
            snd("core.ok1");
            exec(homepage, 16);
        }
        if (index == 4)
        {
            snd("core.ok1");
            set_option();
            return MainMenuResult::main_title_menu;
        }
        if (index == 5)
        {
            snd("core.ok1");
            return MainMenuResult::finish_elona;
        }
    }
}

MainMenuResult main_menu_wrapper()
{
    // Start off in the title menu.
    MainMenuResult result = main_title_menu();
    bool finished = false;
    while (!finished)
    {
        switch (result)
        {
            // Main menu

        case MainMenuResult::main_menu_incarnate:
            result = main_menu_incarnate();
            break;
        case MainMenuResult::main_menu_continue:
            result = main_menu_continue();
            break;
        case MainMenuResult::main_menu_new_game:
            result = main_menu_new_game();
            break;
        case MainMenuResult::main_title_menu:
            // Loop back to the start.
            result = MainMenuResult::main_title_menu;
            finished = true;
            break;

            // Character making

        case MainMenuResult::character_making_select_race:
            result = character_making_select_race();
            break;
        case MainMenuResult::character_making_select_sex:
            result = character_making_select_sex();
            break;
        case MainMenuResult::character_making_select_sex_looped:
            result = character_making_select_sex(false);
            break;
        case MainMenuResult::character_making_select_class:
            result = character_making_select_class();
            break;
        case MainMenuResult::character_making_select_class_looped:
            result = character_making_select_class(false);
            break;
        case MainMenuResult::character_making_role_attributes:
            result = character_making_role_attributes();
            break;
        case MainMenuResult::character_making_role_attributes_looped:
            result = character_making_role_attributes(false);
            break;
        case MainMenuResult::character_making_select_feats:
            result = character_making_select_feats();
            break;
        case MainMenuResult::character_making_select_alias:
            result = character_making_select_alias();
            break;
        case MainMenuResult::character_making_select_alias_looped:
            result = character_making_select_alias(false);
            break;
        case MainMenuResult::character_making_customize_appearance:
            result = character_making_customize_appearance();
            break;
        case MainMenuResult::character_making_final_phase:
            result = character_making_final_phase();
            break;

            // Finished initializing, start the game.

        case MainMenuResult::initialize_game:
            result = MainMenuResult::initialize_game;
            finished = true;
            break;
        case MainMenuResult::finish_elona:
            result = MainMenuResult::finish_elona;
            finished = true;
            break;
        default: assert(0); break;
        }
    }
    return result;
}


MainMenuResult main_menu_new_game()
{
    if (Config::instance().wizard)
    {
        game_data.wizard = 1;
    }
    if (geneuse != ""s)
    {
        load_gene_files();
    }
    rc = 0;
    mode = 1;
    cm = 1;
    gsel(4);
    pos(0, 0);
    picload(filesystem::dir::graphic() / u8"void.bmp", 1);
    gcopy(4, 0, 0, 800, 600, windoww, windowh);
    load_background_variants(2);
    gsel(3);
    pos(960, 96);
    picload(filesystem::dir::graphic() / u8"deco_cm.bmp", 1);
    gsel(0);
    return MainMenuResult::character_making_select_race;
}



MainMenuResult main_menu_continue()
{
    int save_data_count = 0;
    int index = 0;
    cs = 0;
    cs_bk = -1;
    page = 0;
    pagesize = 5;
    keyrange = 0;

    for (const auto& entry : filesystem::dir_entries(
             filesystem::dir::save(), filesystem::DirEntryRange::Type::dir))
    {
        s = filesystem::to_utf8_path(entry.path().filename());
        const auto header_filepath = filesystem::dir::save(s) / u8"header.txt";
        if (!fs::exists(header_filepath))
        {
            continue;
        }

        // the number of bytes read by bload depends on the size of
        // the string passed in, so make it 100 bytes long.
        playerheader = std::string(100, '\0');
        bload(header_filepath, playerheader);

        list(0, save_data_count) = save_data_count;
        listn(0, save_data_count) = s;
        listn(1, save_data_count) = ""s + playerheader;
        key_list(save_data_count) = key_select(save_data_count);
        ++save_data_count;
    }
    listmax = save_data_count;

    while (true)
    {
    savegame_change_page:
        cs_bk = -1;
        pagemax = (listmax - 1) / pagesize;
        if (page < 0)
        {
            page = pagemax;
        }
        else if (page > pagemax)
        {
            page = 0;
        }

        clear_background_in_continue();
        if (jp)
        {
            s = u8"どの冒険を再開するんだい？"s;
        }
        else
        {
            s = u8"Which save game do you want to continue?"s;
        }
        draw_caption();
        windowshadow = 1;
    savegame_draw_page:
        if (jp)
        {
            s(0) = u8"冒険者の選択"s;
            s(1) = u8"BackSpace [削除]  "s + strhint2 + strhint3b;
        }
        else
        {
            s(0) = u8"Game Selection"s;
            s(1) = u8"BackSpace [Delete]  "s + strhint2 + strhint3b;
        }
        display_window(
            (windoww - 440) / 2 + inf_screenx, winposy(288, 1), 440, 288);
        cs_listbk();
        keyrange = 0;
        for (int cnt = 0, cnt_end = pagesize; cnt < cnt_end; ++cnt)
        {
            index = pagesize * page + cnt;
            if (index >= listmax)
            {
                break;
            }
            x = wx + 20;
            y = cnt * 40 + wy + 50;
            display_key(x + 20, y - 2, cnt);
            font(11 - en * 2);
            pos(x + 48, y - 4);
            mes(listn(0, index));
            font(13 - en * 2);
            cs_list(cs == cnt, listn(1, index), x + 48, y + 8);
            ++keyrange;
        }
        cs_bk = cs;
        if (save_data_count == 0)
        {
            font(14 - en * 2);
            pos(wx + 140, wy + 120);
            mes(u8"No save files found"s);
        }
        redraw();

        int cursor{};
        auto action = cursor_check_ex(cursor);

        p = -1;
        for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
        {
            index = pagesize * page + cnt;
            if (index >= listmax)
            {
                break;
            }
            if (cursor == cnt)
            {
                p = list(0, index);
                break;
            }
        }
        if (p != -1)
        {
            playerid = listn(0, p);
            snd("core.ok1");
            mode = 3;
            return MainMenuResult::initialize_game;
        }
        if (ginfo(2) == 0)
        {
            if (save_data_count != 0)
            {
                if (getkey(snail::Key::backspace))
                {
                    p = list(0, cs);
                    playerid = listn(0, p);
                    if (jp)
                    {
                        s = u8"本当に"s + playerid
                            + u8"を削除していいのかい？"s;
                    }
                    else
                    {
                        s = u8"Do you really want to delete "s + playerid
                            + u8" ?"s;
                    }
                    draw_caption();
                    rtval = yes_or_no(promptx, prompty, 200);
                    if (rtval != 0)
                    {
                        return MainMenuResult::main_menu_continue;
                    }
                    if (jp)
                    {
                        s = u8"本当の本当に"s + playerid
                            + u8"を削除していいのかい？"s;
                    }
                    else
                    {
                        s = u8"Are you sure you really want to delete "s
                            + playerid + u8" ?"s;
                    }
                    draw_caption();
                    rtval = yes_or_no(promptx, prompty, 200);
                    if (rtval == 0)
                    {
                        snd("core.ok1");
                        ctrl_file(FileOperation::save_game_delete);
                    }
                    return MainMenuResult::main_menu_continue;
                }
            }
        }
        if (action == "next_page")
        {
            if (pagemax != 0)
            {
                snd("core.pop1");
                ++page;
                goto savegame_change_page;
            }
        }
        if (action == "previous_page")
        {
            if (pagemax != 0)
            {
                snd("core.pop1");
                --page;
                goto savegame_change_page;
            }
        }
        if (action == "cancel")
        {
            return MainMenuResult::main_title_menu;
        }
        goto savegame_draw_page;
    }
}



MainMenuResult main_menu_incarnate()
{
    cs = 0;
    cs_bk = -1;
    gsel(4);
    pos(0, 0);
    picload(filesystem::dir::graphic() / u8"void.bmp", 1);
    gcopy(4, 0, 0, 800, 600, windoww, windowh);
    gsel(0);
    gmode(0);
    pos(0, 0);
    gcopy(4, 0, 0, windoww, windowh);
    gmode(2);
    if (jp)
    {
        s = u8"どの遺伝子を引き継ぐ？"s;
    }
    else
    {
        s = u8"Which gene do you want to incarnate?"s;
    }
    draw_caption();
    keyrange = 0;
    listmax = 0;
    for (const auto& entry : filesystem::dir_entries(
             filesystem::dir::save(), filesystem::DirEntryRange::Type::dir))
    {
        s = filesystem::to_utf8_path(entry.path().filename());
        const auto gene_header_filepath =
            filesystem::dir::save(s) / u8"gene_header.txt";
        if (!fs::exists(gene_header_filepath))
        {
            continue;
        }
        bload(gene_header_filepath, playerheader);
        list(0, listmax) = listmax;
        listn(0, listmax) = s;
        listn(1, listmax) = ""s + playerheader;
        key_list(listmax) = key_select(listmax);
        ++keyrange;
        ++listmax;
    }
    windowshadow = 1;

    while (1)
    {
        if (jp)
        {
            s(0) = u8"遺伝子の選択"s;
            s(1) = strhint3b;
        }
        else
        {
            s(0) = u8"Gene Selection"s;
            s(1) = strhint3b;
        }
        display_window(
            (windoww - 440) / 2 + inf_screenx, winposy(288, 1), 440, 288);
        cs_listbk();
        for (int cnt = 0, cnt_end = (listmax); cnt < cnt_end; ++cnt)
        {
            x = wx + 20;
            y = cnt * 40 + wy + 50;
            display_key(x + 20, y - 2, cnt);
            font(11 - en * 2);
            pos(x + 48, y - 4);
            mes(listn(0, cnt));
            font(13 - en * 2);
            cs_list(cs == cnt, listn(1, cnt), x + 48, y + 8);
        }
        cs_bk = cs;
        if (listmax == 0)
        {
            font(14 - en * 2);
            pos(wx + 140, wy + 120);
            mes(u8"No gene files found"s);
        }
        redraw();

        int cursor{};
        auto command = cursor_check_ex(cursor);

        p = -1;
        for (int cnt = 0, cnt_end = (keyrange); cnt < cnt_end; ++cnt)
        {
            if (cnt == cursor)
            {
                p = list(0, cnt);
                break;
            }
        }
        if (p != -1)
        {
            snd("core.ok1");
            geneuse = listn(0, p);
            playerid = listn(0, p);
            return MainMenuResult::main_menu_new_game;
        }
        if (command == "cancel")
        {
            return MainMenuResult::main_title_menu;
        }
    }
    return MainMenuResult::main_title_menu;
}

} // namespace elona
