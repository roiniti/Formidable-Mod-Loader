function ExitCustom()
	pg.TipsMgr.GetInstance():ShowTips('Estoy Adentro')
	if false and not IsNil(pg.MsgboxMgr.GetInstance()._go) then
		pg.MsgboxMgr.GetInstance():ShowMsgBox({
			content = i18n("confirm_app_exit"),
			onYes = function ()
				Application.Quit()
			end
		})
	end
end
ExitCustom()